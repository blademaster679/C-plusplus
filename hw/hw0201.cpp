#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> one_word = {"","one","two","three","four","five","six","seven","eight","nine","ten",
                           "eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
vector<string> ten_word = {"","","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

string NumToWord(long long n , string suffix){
    if (n == 0) {
        return "";
    }else if(n > 0 && n < 20){
        return one_word[n] + " " + suffix;
    }else if(n >= 20 && n < 100){
        return ten_word[n/10] + "-" + one_word[n%10] + suffix;
    }else{
        return one_word[n/100] + " hundred " + NumToWord(n%100 , "") + " " + suffix;
    }
}
/*问题 4: 零的处理可能不准确
如果某个数量级为零，例如输入为 1000000，结果可能多出空格或不必要的拼接。
修复方法：在拼接每个数量级（billion、million、thousand）时，检查是否为零，如果是零就跳过，不添加对应的单词。
*/

string ConvertToNum(long long n){
    if(n == 0){
        return "zero";
    }
    string results = "";
    if(n < 0){
        results += "negative ";
        n = -n;
    }
    results += NumToWord(n/1000000000 , "billion ");
    results += NumToWord((n/1000000)%1000 , "million ");
    results += NumToWord((n/1000)%1000 , "thousand ");
    results += NumToWord((n/100)%10 , "hundred ");
    /*在 ConvertToNum 中，处理了 (n / 100) % 10 的百位情况，但 NumToWord 函数已经在处理百位时拼接了 " hundred"，此处会导致重复，
    例如：123 输出为 "one hundred hundred twenty three"。
     修复方法：删除 ConvertToNum 中 (n / 100) % 10 的单独处理，因为这个逻辑已经由 NumToWord 函数处理。*/
    results += NumToWord(n%100 , "");
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
    cout << ConvertToNum(n) << endl;
    return 0;
}
//有两个大问题，答案在hw0201gpt.cpp中，注释也明确了问题所在。