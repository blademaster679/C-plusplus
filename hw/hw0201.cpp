#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> one_word = {"","one","two","three","four","five","six","seven","eight","nine","ten",
                           "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
vector<string> ten_word = {"","","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

string NumToWord(int num , string suffix){
    if (num == 0) {
        return "";
    }else if(num > 0 && num < 20){
        return one_word[num] + " " + suffix;
    }else if(num >= 20 && num < 100){
        return ten_word[num/10] + "-" + one_word[num%10] + suffix;
    }else{
        return one_word[num/100] + " hundred " + NumToWord(num%100 , "") + " " + suffix;
    }
}

string ConvertToNum(long long num){
    if(num == 0){
        return "zero";
    }
    string results = "";
    if(num < 0){
        results += "negative ";
        num = -num;
    }
    results += NumToWord(num/1000000000 , "billion ");
    results += NumToWord((num/1000000)%1000 , "million ");
    results += NumToWord((num/1000)%1000 , "thousand ");
    results += NumToWord((num/100)%10 , "hundred ");
    results += NumToWord(num%100 , "");
    // check and remove the last space and dash
    while (results.back() == ' ') {
        results.pop_back();
    }
    while (results.back() == '-')
    {
        results.pop_back();
    }
    return results;
}

int main(){
    long long num;
    cin >> num;
    cout << ConvertToNum(num) << endl;
    return 0;
}