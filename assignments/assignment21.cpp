#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

int main()
{
    int x1, y1, x2, y2;
    double distance;
    std::cin >> x1 >> y1 >> x2 >> y2;
    distance= sqrt(pow(abs(x1-x2),2)+pow(abs(y1-y2),2));
    std::cout << std::fixed << std::setprecision(6) << distance <<std::endl;
}