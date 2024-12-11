#include <iostream>
#include <string>
#include "removespace.hpp"
// Function to remove spaces from a string
std::string removeSpaces(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (char c : str) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            result += c;
        }
    }
    return result;
}
