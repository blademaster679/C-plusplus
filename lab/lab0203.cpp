#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
using namespace std;

int main()
{
    double a = 0;
    double b = 0;
    double c = 0;
    double x1 = 0;
    double x2 = 0;
    double judge = 0;
    double realpart = 0;
    double imaginarypart = 0;

    cin >> a >> b >> c;
    judge = (pow(b, 2) - 4 * a * c);
    if (judge > 0)
    {
        x1 = (-b + sqrt(judge)) / (2 * a);
        x2 = (-b - sqrt(judge)) / (2 * a);
        cout << "Distinct real roots" << "\n" << x1 << "\n"
             << x2 << endl;
    }
    else if (judge == 0)
    {
        x1 = x2 = (-b / (2 * a));
        cout << "Identical real roots" <<"\n"<< x1 << endl;
    }
    else
    {
        realpart = (-b / (2 * a));
        imaginarypart = (sqrt(-judge) / (2 * a));
        cout << "Complex roots" << "\n"
             << fixed << setprecision(2) << realpart <<" "<< "+" << " "<< fixed << setprecision(3) << imaginarypart << "i" << "\n"
             << fixed << setprecision(2) << realpart <<" "<< "-" <<" " <<fixed << setprecision(3) << imaginarypart << "i" << endl;
    }
    return 0;
}