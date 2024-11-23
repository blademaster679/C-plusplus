#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 数字到英文单词的映射
vector<string> one_word = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
                           "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
vector<string> ten_word = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};

// 将三位数及以下的数字转换为英文
string NumToWord(long long n, string suffix) {
    string result = "";
    if (n == 0) {
        return "";
    } else if (n < 20) {
        result = one_word[n];
    } else if (n < 100) {
        result = ten_word[n / 10];
        if (n % 10) result += "-" + one_word[n % 10]; // 在十位和个位之间加上 "-"
    } else {
        result = one_word[n / 100] + " hundred";
        if (n % 100) result += " " + NumToWord(n % 100, "");//如果百位后还有数字，递归处理
    }
    return result + (suffix.empty() ? "" : " " + suffix);
}

// 将完整数字转换为英文
string ConvertToNum(long long n) {
    if (n == 0) {
        return "zero";
    }

    string result = "";
    if (n < 0) {
        result += "negative ";
        n = -n;
    }

    // 分别处理 billion, million, thousand, 和 hundred 的部分
    if (n / 1000000000) result += NumToWord(n / 1000000000, "billion") + " ";
    if ((n / 1000000) % 1000) result += NumToWord((n / 1000000) % 1000, "million") + " ";
    if ((n / 1000) % 1000) result += NumToWord((n / 1000) % 1000, "thousand") + " ";
    if (n % 1000) result += NumToWord(n % 1000, "");

    // 移除结果末尾多余的空格
    while (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    return result;
}

int main() {
    long long n;
    cin >> n;
    cout << ConvertToNum(n) << endl;
    return 0;
}
