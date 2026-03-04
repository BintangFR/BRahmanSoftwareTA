#include "../build-mingw/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cstring>

#define UNIT_TEST
#include "Question-2.cc"

TEST(SimpleTaskTest, DoublesInputDuringProcess) {
	SimpleTask task(12.5f);
	task.process();

	EXPECT_FLOAT_EQ(task.getProcessedValue(), 25.0f);
	EXPECT_EQ(task.getTaskType(), 0);
}

TEST(ComplexTaskTest, SumsVectorDuringProcess) {
	ComplexTask task(std::vector<int>{3, 4, 5, -2});
	task.process();

	EXPECT_FLOAT_EQ(task.getProcessedValue(), 10.0f);
	EXPECT_EQ(task.getTaskType(), 1);
}

TEST(PacketPackingTest, PacksSimpleTaskAsExpected) {
	std::atomic<bool> shutdown{false};
	ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;
	PacketTransmitter transmitter(processed_queue, shutdown);

	SimpleTask task(10.0f);
	task.process();

	uint8_t buffer[8] = {0};
	transmitter.packTask(task, 100U, buffer);

	EXPECT_EQ(buffer[0], 0x00);
	EXPECT_EQ(buffer[1], 0x41);
	EXPECT_EQ(buffer[2], 0xA0);
	EXPECT_EQ(buffer[3], 0x00);
	EXPECT_EQ(buffer[4], 0x00);
	EXPECT_EQ(buffer[5], 0x00);
	EXPECT_EQ(buffer[6], 0x00);
	EXPECT_EQ(buffer[7], 0x64);
}

TEST(PacketPackingTest, PacksComplexTaskAndTimestampLower24Bits) {
	std::atomic<bool> shutdown{false};
	ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;
	PacketTransmitter transmitter(processed_queue, shutdown);

	ComplexTask task(std::vector<int>{10, 20, 30});
	task.process();

	uint8_t buffer[8] = {0};
	transmitter.packTask(task, 0x12ABCDEFU, buffer);

	EXPECT_EQ(buffer[0], 0x40);
	EXPECT_EQ(buffer[1], 0x00);
	EXPECT_EQ(buffer[2], 0x00);
	EXPECT_EQ(buffer[3], 0x00);
	EXPECT_EQ(buffer[4], 0x3C);
	EXPECT_EQ(buffer[5], 0xAB);
	EXPECT_EQ(buffer[6], 0xCD);
	EXPECT_EQ(buffer[7], 0xEF);
}
