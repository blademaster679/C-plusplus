/*#include <iostream>
#include <string>

size_t find(const std::string &src, const std::string &pattern, int start) {
    // Check for valid start index
    if (start < 0 || start >= static_cast<int>(src.length())) {
        return std::string::npos; // Return not found
    }

    // Lengths of the source string and pattern
    size_t srcLength = src.length();
    size_t patternLength = pattern.length();

    // Search for the pattern in the source string
    for (size_t i = start; i <= srcLength - patternLength; ++i) {
        // Compare the substring with the pattern
        if (src.substr(i, patternLength) == pattern) {
            return i; // Return the index of the first occurrence
        }
    }

    return std::string::npos; // Return not found
}

int main() {
    std::string src = "Hello, world! Hello again!";
    std::string pattern = "Hello";
    int start = 0;

    size_t index = find(src, pattern, start);
    if (index != std::string::npos) {
        std::cout << "Pattern found at index: " << index << std::endl;
    } else {
        std::cout << "Pattern not found." << std::endl;
    }

    return 0;
}
*/

/*#include <iostream>
#include <string>

size_t find_first_of(const std::string &src, const std::string &pattern, int start) {
    // Check for valid start index
    if (start < 0 || start >= static_cast<int>(src.length())) {
        return std::string::npos; // Return not found
    }

    // Search for any character in pattern in the source string
    for (size_t i = start; i < src.length(); ++i) {
        for (char ch : pattern) {
            if (src[i] == ch) {
                return i; // Return the index of the first occurrence
            }
        }
    }

    return std::string::npos; // Return not found
}

int main() {
    std::string src = "Hello, world!";
    std::string pattern = "aeiou"; // Vowels
    int start = 0;

    size_t index = find_first_of(src, pattern, start);
    if (index != std::string::npos) {
        std::cout << "First occurrence found at index: " << index << std::endl;
    } else {
        std::cout << "No characters from pattern found." << std::endl;
    }

    return 0;
}
*/

#include <iostream>
#include <string>

std::string replace(const std::string &src, const std::string &pattern, int start, int size) {
    // Check for valid start index and size
    if (start < 0 || start >= static_cast<int>(src.length()) || size < 0 || start + size > static_cast<int>(src.length())) {
        return src; // Return the original string if parameters are invalid
    }

    // Create a new string with the replacement
    std::string result = src.substr(0, start); // Part before the replacement
    result += pattern; // Add the new pattern
    result += src.substr(start + size); // Part after the replacement

    return result; // Return the modified string
}

int main() {
    std::string src = "Hello, world!";
    std::string pattern = "C++";
    int start = 7; // Starting index of "world"
    int size = 5; // Size of "world"

    std::string newStr = replace(src, pattern, start, size);
    std::cout << "Modified string: " << newStr << std::endl;

    return 0;
}
