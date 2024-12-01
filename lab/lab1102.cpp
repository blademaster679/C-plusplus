#include <iostream>
#include <unordered_map>
using namespace std;

int SumOfSquare(int n){
    int sum = 0;
    while (n > 0){
        int digit = n % 10;
        sum += digit * digit;
        n /= 10;
    }
    return sum;
}

bool isHappy(int n){
    unordered_map<int, bool> visited;
    while (n != 1 && !visited[n]){
        visited[n] = true;
        n = SumOfSquare(n);
    }
    return n == 1;
}

int main(){
    int n;
    cin >> n;
    if (isHappy(n)){
        cout << "True" << endl;
    } else {
        cout << "False" << endl;
    }
}