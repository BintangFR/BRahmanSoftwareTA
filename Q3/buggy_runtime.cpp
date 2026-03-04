// buggy_runtime.cpp
// This file compiles successfully but contains intentional RUNTIME errors.
// Your task: identify and fix all runtime bugs.
//
// Compile: g++ -Wall -Wextra -g -o buggy_runtime buggy_runtime.cpp
// Run:     ./buggy_runtime
//
// Recommended: use AddressSanitizer to catch memory errors
// Compile: g++ -fsanitize=address -g -o buggy_runtime_asan buggy_runtime.cpp
// Run:     ./buggy_runtime_asan

#include <iostream>
#include <vector>
#include <string>

// Bug 1: Out-of-bounds array access
void printSensorReadings() {
    int readings[5] = {10, 20, 30, 40, 50};
    std::cout << "Sensor readings:\n";
    for (int i = 0; i <= 5; ++i) {   // off-by-one: should be i < 5
        std::cout << "  [" << i << "] = " << readings[i] << "\n";
    }
}

// Bug 2: Division by zero
double computeAverageFuelEfficiency(double totalDistance, double fuelUsed) {
    // fuelUsed can be 0 – no guard against division by zero
    return totalDistance / fuelUsed;
}

// Bug 3: Null pointer dereference
void printVehicleId() {
    std::string* vehicleId = nullptr;
    std::cout << "Vehicle ID: " << *vehicleId << "\n";  // dereferences null pointer
}

int main() {
    std::cout << "=== Runtime Bug Demo ===\n\n";

    // Triggers Bug 1
    printSensorReadings();

    // Triggers Bug 2
    double efficiency = computeAverageFuelEfficiency(150.0, 0.0);
    std::cout << "\nFuel efficiency: " << efficiency << " km/L\n";

    // Triggers Bug 3
    printVehicleId();

    return 0;
}
