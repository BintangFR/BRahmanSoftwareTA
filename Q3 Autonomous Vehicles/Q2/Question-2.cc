#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <iomanip>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdint>
#include <cstring>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        cv_.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return !queue_.empty(); });

        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }
    size_t size() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
    // A non-blocking pop for graceful shutdown
    T pop_for_shutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return T{};
        }

        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }
};

// Class Representation of a Task
class ITask {
public:
    virtual ~ITask() = default;
    virtual void process() = 0;
    virtual float getProcessedValue() const = 0;
    virtual uint8_t getTaskType() const = 0;
};

class SimpleTask : public ITask {
private:
    float value_;
public:
    explicit SimpleTask(float val) : value_(val) {}

    void process() override {
        value_ *= 2.0f;
    }

    float getProcessedValue() const override {
        return value_;
    }

    uint8_t getTaskType() const override {
        return 0;
    }
};

class ComplexTask : public ITask {
private:
    std::vector<int> values_;
    int sum_ = 0;
public:
    explicit ComplexTask(std::vector<int> nums) : values_(std::move(nums)) {}

    void process() override {
        int total = 0;
        for (int number : values_) {
            total += number;
        }
        sum_ = total;
    }

    float getProcessedValue() const override {
        return static_cast<float>(sum_);
    }

    uint8_t getTaskType() const override {
        return 1;
    }
};


class TaskGenerator {
private:
    ThreadSafeQueue<std::unique_ptr<ITask>>& task_queue_;
    std::atomic<bool>& shutdown_;
public:
    TaskGenerator(ThreadSafeQueue<std::unique_ptr<ITask>>& queue, std::atomic<bool>& shutdown)
        : task_queue_(queue), shutdown_(shutdown) {}
    void run() {
        int counter = 0;
        while (!shutdown_.load()) {
            if ((counter % 2) == 0) {
                float value = static_cast<float>((counter % 50) + 1);
                task_queue_.push(std::make_unique<SimpleTask>(value));
            } else {
                std::vector<int> values{counter, counter + 1, counter + 2};
                task_queue_.push(std::make_unique<ComplexTask>(values));
            }

            ++counter;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
};

class TaskProcessor {
private:
    ThreadSafeQueue<std::unique_ptr<ITask>>& task_queue_;
    ThreadSafeQueue<std::unique_ptr<ITask>>& processed_queue_;
    std::atomic<bool>& shutdown_;
public:
    TaskProcessor(ThreadSafeQueue<std::unique_ptr<ITask>>& t_queue, ThreadSafeQueue<std::unique_ptr<ITask>>& p_queue, std::atomic<bool>& shutdown)
        : task_queue_(t_queue), processed_queue_(p_queue), shutdown_(shutdown) {}
    void run() {
        while (true) {
            std::unique_ptr<ITask> task = task_queue_.pop_for_shutdown();
            if (task) {
                task->process();
                processed_queue_.push(std::move(task));
                continue;
            }

            if (shutdown_.load() && task_queue_.size() == 0) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
};

class PacketTransmitter {
private:
    ThreadSafeQueue<std::unique_ptr<ITask>>& processed_queue_;
    std::atomic<bool>& shutdown_;
public:
    PacketTransmitter(ThreadSafeQueue<std::unique_ptr<ITask>>& queue, std::atomic<bool>& shutdown)
        : processed_queue_(queue), shutdown_(shutdown) {}
    void run(std::ostream& os = std::cout) {
        while (true) {
            std::unique_ptr<ITask> processed_task = processed_queue_.pop_for_shutdown();
            if (processed_task) {
                transmit(processed_task, os);
                continue;
            }

            if (shutdown_.load() && processed_queue_.size() == 0) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    void packTask(const ITask& data, uint32_t timestamp, uint8_t buffer[8]) {
        std::memset(buffer, 0, 8);

        const uint8_t task_type = static_cast<uint8_t>(data.getTaskType() & 0x03U);
        buffer[0] = static_cast<uint8_t>(task_type << 6);

        if (task_type == 0U) {
            float processed_float = data.getProcessedValue();
            uint32_t float_bits = 0;
            std::memcpy(&float_bits, &processed_float, sizeof(float_bits));
            buffer[1] = static_cast<uint8_t>((float_bits >> 24) & 0xFFU);
            buffer[2] = static_cast<uint8_t>((float_bits >> 16) & 0xFFU);
            buffer[3] = static_cast<uint8_t>((float_bits >> 8) & 0xFFU);
            buffer[4] = static_cast<uint8_t>(float_bits & 0xFFU);
        } else {
            int32_t processed_int = static_cast<int32_t>(data.getProcessedValue());
            uint32_t int_bits = static_cast<uint32_t>(processed_int);
            buffer[1] = static_cast<uint8_t>((int_bits >> 24) & 0xFFU);
            buffer[2] = static_cast<uint8_t>((int_bits >> 16) & 0xFFU);
            buffer[3] = static_cast<uint8_t>((int_bits >> 8) & 0xFFU);
            buffer[4] = static_cast<uint8_t>(int_bits & 0xFFU);
        }

        const uint32_t ts24 = timestamp & 0x00FFFFFFU;
        buffer[5] = static_cast<uint8_t>((ts24 >> 16) & 0xFFU);
        buffer[6] = static_cast<uint8_t>((ts24 >> 8) & 0xFFU);
        buffer[7] = static_cast<uint8_t>(ts24 & 0xFFU);
    }

    void transmit(const std::unique_ptr<ITask>& data, std::ostream& os) {
        uint8_t buffer[8] = {0};

        const auto now = std::chrono::system_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        const uint32_t timestamp = static_cast<uint32_t>(ms);

        packTask(*data, timestamp, buffer);

        os << "Packet: ";
        for (int i = 0; i < 8; ++i) {
            os << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)buffer[i] << " ";
        }
        os << std::dec << std::endl;
    }
};

#ifndef UNIT_TEST
int main() {
    std::cout << "Starting the data generation pipeline" << std::endl;

    std::atomic<bool> shutdown_flag{false};

    ThreadSafeQueue<std::unique_ptr<ITask>> task_queue;
    ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;

    TaskGenerator generator(task_queue, shutdown_flag);
    TaskProcessor processor(task_queue, processed_queue, shutdown_flag);
    PacketTransmitter transmitter(processed_queue, shutdown_flag);

    std::thread generator_thread(&TaskGenerator::run, &generator);
    std::thread processor_thread(&TaskProcessor::run, &processor);
    std::thread transmitter_thread(&PacketTransmitter::run, &transmitter, std::ref(std::cout));

    std::this_thread::sleep_for(std::chrono::seconds(10));

    shutdown_flag = true;

    generator_thread.join();
    processor_thread.join();
    transmitter_thread.join();

    std::cout << "Data Gen pipeline Finished." << std::endl;

    return 0;
}
#endif
