#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

// Function to calculate the shift value based on the keyword
int calculateShift(const std::string& keyword) {
    int shift = 0;
    for (char ch : keyword) {
        shift += static_cast<int>(ch); // Sum ASCII values of characters in the keyword
    }
    return shift % 26; // Use modulo 26 to keep shift within alphabet range
}

// Function to encrypt a single character using Caesar Cipher
char encryptChar(char ch, int shift) {
    if (ch >= 'A' && ch <= 'Z') { // Uppercase letters
        return 'A' + (ch - 'A' + shift) % 26;
    } else if (ch >= 'a' && ch <= 'z') { // Lowercase letters
        return 'a' + (ch - 'a' + shift) % 26;
    }
    // Non-alphabet characters remain unchanged
    return ch;
}

// Function to decrypt a single character using Caesar Cipher
char decryptChar(char ch, int shift) {
    if (ch >= 'A' && ch <= 'Z') { // Uppercase letters
        return 'A' + (ch - 'A' - shift + 26) % 26;
    } else if (ch >= 'a' && ch <= 'z') { // Lowercase letters
        return 'a' + (ch - 'a' - shift + 26) % 26;
    }
    // Non-alphabet characters remain unchanged
    return ch;
}

// Function to encrypt or decrypt the entire text in the file
void processFile(const std::string& inputFile, const std::string& outputFile, const std::string& keyword, bool isEncrypt) {
    int shift = calculateShift(keyword); // Calculate shift based on keyword

    std::ifstream inFile(inputFile);
    std::ofstream outFile(outputFile);

    if (!inFile || !outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    char ch;
    while (inFile.get(ch)) { // Read each character from input file
        if (isEncrypt) {
            outFile.put(encryptChar(ch, shift)); // Encrypt and write to output file
        } else {
            outFile.put(decryptChar(ch, shift)); // Decrypt and write to output file
        }
    }

    std::cout << (isEncrypt ? "Encryption" : "Decryption") << " complete! Result saved as " << outputFile << std::endl;

    inFile.close();
    outFile.close();
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <encrypt/decrypt> <input file> <output file> <keyword>" << std::endl;
        return 1;
    }

    // Determine whether to encrypt or decrypt based on the first argument
    bool isEncrypt = std::string(argv[1]) == "encrypt";
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    std::string keyword = argv[4];

    // Process the file (encrypt or decrypt)
    processFile(inputFile, outputFile, keyword, isEncrypt);

    return 0;
}

