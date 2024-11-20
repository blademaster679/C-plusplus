#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string multiply(const string &num1, const string &num2){
    if (num1 == "0" || num2 == "0") return "0";
    int len1 = num1.length();
    int len2 = num2.length();
    vector<int> result(len1 + len2, 0);
    for (int i = len1-1; i >= 0; i--){
        int n1 = num1[i] - '0';
        for (int j = len2-1; j >= 0 ;j--){
            int n2 = num2[j] - '0';
            int sum = n1 * n2 + result[i+j+1];
            result[i+j+1] = sum % 10; //当前为
            result[i+j] += sum / 10; //进位
        }
    }
    string results = "";
    int i = 0;
    while(i <result.size() && result[i] == 0) i ++;
    for (;i < result.size(); i++){
        results += result[i] + '0';
    }
    return results;
}

int main(){
    ifstream input("input.txt");
    ofstream output("output.txt");
    string num1, num2;
    getline(input, num1);
    getline(input, num2);
    string result = multiply(num1, num2);
    output << result << endl;
    input.close();
    output.close();
    return 0;
}