#include <iostream>


int main()
{
    int a = 0;
    std::cin >> a ;
    if (a%2==0)
    {
        std::cout << "Even" << std::endl;
    }else{
        std::cout << "Odd" << std::endl;
    }
return 0;
}