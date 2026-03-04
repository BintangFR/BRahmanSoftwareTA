// buggy_compile.cpp
// This file contains intentional COMPILE-TIME errors.
// Your task: identify and fix all errors so the file compiles cleanly.
//
// Hint: there are at least 4 distinct errors below.
// Run: g++ -Wall -Wextra -o buggy_compile buggy_compile.cpp

#include <iostream>
#include <string>

// Error 1: missing return type on function declaration
greet(const std::string& name) {
    std::cout << "Hello, " << name << "!\n";
}

// Error 2: wrong type – trying to assign a string literal to an int
int getSpeed() {
    int speed = "100";  // should be an integer literal
    return speed;
}

// Error 3: undeclared variable used in expression
double computeAverage() {
    double total = 0.0;
    int count = 5;
    return total / count + extra;  // 'extra' is not declared
}

int main() {
    greet("Driver");

    int s = getSpeed();
    std::cout << "Speed: " << s << " km/h\n";

    double avg = computeAverage();
    std::cout << "Average: " << avg << "\n"

    // Error 4: missing semicolon on the line above (before the closing brace)
    return 0;
}
