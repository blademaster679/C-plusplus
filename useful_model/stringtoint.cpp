#include <sstream>
#include <iostream>
#include <string>
using namespace std;

int string_to_int(const string& s){
    istringstream ss(s);
    int n;
    ss >> n;
    return n;
}

int main(){
    string s = "123";
    int n = string_to_int(s);
    cout << n << endl;
    return 0;
}