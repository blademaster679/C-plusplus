#include <iostream>
using namespace std;


int main()
{
    int a = 0;
    int factorial = 1;
    cin >> a;
    if (a!=0){
        while (a>0){
            factorial = factorial*a;
            a--;
        }
    }else{
        factorial = 1;
    }
cout << factorial;
return 0;
}