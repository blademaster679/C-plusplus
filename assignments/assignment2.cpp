#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

int main()
{
    int r;
    //std::cout <<"please input r between 0 and 500:";
    std::cin >> r;
    double V;
    double S;
    const double PIE = 3.14159265358979;
    V = (4.0/3.0) * PIE * pow(r,3);
    S = 4 * PIE * pow(r,2);
    std::cout << std::fixed << std::setprecision(2) << V << "\n" << S << std::endl;
}