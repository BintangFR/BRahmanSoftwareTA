#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>

// Question 1: This is an extension task that requires you to decode sensor data from a CAN log file.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the "WheelSpeedRR" values
// from the candump.log file. Parse these values correctly and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the expected data output format.
// You do not need to use any external libraries. Use the resources below to understand how to extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro

int main(){
    // Open the CAN log input file for reading.
    std::ifstream infile("candump.log");
    // Open the output file where decoded WheelSpeedRR values will be written.
    std::ofstream outfile("output.txt");
    // Reusable buffer to hold each log line read from the input file.
    std::string line;

    // Exit with an error code if either file failed to open.
    if (!infile.is_open() || !outfile.is_open()) {
        return 1;
    }

    // Process the CAN log one line at a time.
    while (std::getline(infile, line)) {
        // Create a stream so we can split this line into whitespace-separated fields.
        std::istringstream line_stream(line);
        // Field 1: timestamp text, e.g. "(1705638753.913408)".
        std::string timestamp;
        // Field 2: CAN interface name, e.g. "vcan0".
        std::string interface_name;
        // Field 3: frame text, e.g. "705#39C2A37B95B17C57".
        std::string frame;

        // Skip malformed lines that do not have exactly these 3 required fields.
        if (!(line_stream >> timestamp >> interface_name >> frame)) {
            continue;
        }

        // Find the separator between CAN ID and payload.
        const std::size_t hash_pos = frame.find('#');
        // Skip invalid frames that do not contain '#'.
        if (hash_pos == std::string::npos) {
            continue;
        }

        // Extract the CAN ID (left side of '#').
        const std::string can_id = frame.substr(0, hash_pos);
        // Extract the payload hex string (right side of '#').
        const std::string payload = frame.substr(hash_pos + 1);

        // Only decode message ID 705 and require at least 6 payload bytes (12 hex chars)
        // because WheelSpeedRR is at bytes 4 and 5.
        if (can_id != "705" || payload.size() < 12) {
            continue;
        }

        // Hold byte 4 (least significant byte for this little-endian signal).
        int byte4 = 0;
        // Hold byte 5 (most significant byte for this little-endian signal).
        int byte5 = 0;
        try {
            // Parse payload byte 4 from hex characters at indices 8..9.
            byte4 = std::stoi(payload.substr(8, 2), nullptr, 16);
            // Parse payload byte 5 from hex characters at indices 10..11.
            byte5 = std::stoi(payload.substr(10, 2), nullptr, 16);
        } catch (...) {
            // Skip this line if hex parsing fails.
            continue;
        }

        // Reconstruct signed 16-bit raw value from little-endian bytes [byte4, byte5].
        const int16_t raw_value = static_cast<int16_t>((byte5 << 8) | byte4);
        // Apply DBC scaling factor (0.1) to get WheelSpeedRR in km/h.
        const double wheel_speed_rr = static_cast<double>(raw_value) * 0.1;

        // Write one decoded line in the required format: (timestamp): value
        // with one digit after the decimal point.
        outfile << timestamp << ": " << std::fixed << std::setprecision(1) << wheel_speed_rr << '\n';
    }

    // Return success after processing all lines.
    return 0;
}
