#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> one_word = {"","one","two","three","four","five","six","seven","eight","nine","ten",
                           "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
vector<string> ten_word = {"","","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

string nToWord(int n , string suffix){
    if (n == 0) {
        return "";
    }else if(n > 0 && n < 20){
        return one_word[n] + " " + suffix;
    }else if(n >= 20 && n < 100){
        return ten_word[n/10] + "-" + one_word[n%10] + suffix;
    }else{
        return one_word[n/100] + " hundred " + nToWord(n%100 , "") + " " + suffix;
    }
}

string ConvertTon(long long n){
    if(n == 0){
        return "zero";
    }
    string results = "";
    if(n < 0){
        results += "negative ";
        n = -n;
    }
    results += nToWord(n/1000000000 , "billion ");
    results += nToWord((n/1000000)%1000 , "million ");
    results += nToWord((n/1000)%1000 , "thousand ");
    results += nToWord((n/100)%10 , "hundred ");
    results += nToWord(n%100 , "");
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
    long long n;
    cin >> n;
    cout << ConvertTon(n) << endl;
    return 0;
}