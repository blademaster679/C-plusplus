#include <iostream>
#include <cmath>
using namespace std;

int GCD(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return GCD(b, a % b);
}

int main()
{
    int a, b;
    cin >> a >> b;
    cout << GCD(a, b) << endl;
}