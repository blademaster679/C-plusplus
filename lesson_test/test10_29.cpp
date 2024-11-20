#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    // ofstream outputFile;
    // outputFile.open("hello.txt");
    // outputFile << "Hello, World!" << endl;
    // outputFile.close();
    std::ifstream file("example.txt");
    std::string line;
    if (getline(file, line))
    {
        cout << line << endl;
        // 现在 line 中包含从文件中读取的整行文本
    }
}