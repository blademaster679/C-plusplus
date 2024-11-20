#include <iostream>
using namespace std;


int main()
{
    long long a = 0;
    long long factorial = 1;
    cin >> a;
    while (a>1){
            factorial = factorial*a;
            a--;
        }
std::cout << factorial;
return 0;
}