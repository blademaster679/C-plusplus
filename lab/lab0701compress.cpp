#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string compress(const string &s){
    string compressed = "";
    int n = s.length();
    for (int i = 0 ; i < n ;){
        char ch = s[i];
        int count = 1;
        while(i + count < n && s[i+count] == ch){
            count++;
        }
        if(count == 1){
            compressed += ch;
        }
        else{
            compressed += ch + to_string(count);
        }
        i += count;
    }
    return compressed;
}

int main(){
    string s;
    getline(cin,s);
    string compressed = compress(s);
    cout << compressed <<endl;
    return 0;
}