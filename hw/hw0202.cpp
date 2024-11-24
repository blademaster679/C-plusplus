#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

const vector<string> words = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
                              "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", 
                              "nineteen", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety", 
                              "hundred", "thousand", "million", "billion"};

const vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 30, 40, 50, 60, 70, 80, 90, 
                            100, 1000, 1000000, 1000000000};

long long GetValue(const string& word){
    for (int i = 0; i < words.size(); i++){
        if(words[i] == word){
            return values[i];
        }
    }
    return -1;
}

long long HandleHyphen(const string& word){
    int hyphen_pos = word.find('-');
    if(hyphen_pos != string::npos){
        string first_part = word.substr(0, hyphen_pos);
        string second_part = word.substr(hyphen_pos+1);
        return GetValue(first_part) + GetValue(second_part);
    }
    return GetValue(word);
}

long long ConvertToNum(const string& input){
    stringstream ss(input);
    string word;
    long long current = 0;
    long long result = 0;
    bool is_negative = false;
    while(ss >> word){
        if(word == "negative"){
            is_negative = true;
        }
        else{
            long long value = HandleHyphen(word);
            if (value == 100){
                current *= 100;
            }else if(value >= 1000){
                current *= value;
                result += current;
                current = 0;
            }else{
                current += value;
            }
        }
    }
    result += current;
    if(is_negative){
        result = -result;
    }
    return result;
}

int main(){
    string input;
    getline(cin, input);
    cout << ConvertToNum(input) << endl;
    return 0;
}