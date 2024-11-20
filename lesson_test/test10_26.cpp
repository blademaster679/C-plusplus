#include <iostream>
int main()
{
    int a = 3;
    int &b = a;
    ++b;
    std::cout << a << b << std::endl;
}
