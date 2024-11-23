#include <iostream>
using namespace std;

// 函数模板定义
template <typename T>
void our_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 10, y = 20;
    cout << "交换前: x = " << x << ", y = " << y << endl;
    our_swap(x, y);  // 自动推导 T 为 int
    cout << "交换后: x = " << x << ", y = " << y << endl;

    double p = 1.1, q = 2.2;
    cout << "交换前: p = " << p << ", q = " << q << endl;
    our_swap(p, q);  // 自动推导 T 为 double
    cout << "交换后: p = " << p << ", q = " << q << endl;

    char c1 = 'A', c2 = 'B';
    cout << "交换前: c1 = " << c1 << ", c2 = " << c2 << endl;
    our_swap(c1, c2);  // 自动推导 T 为 char
    cout << "交换后: c1 = " << c1 << ", c2 = " << c2 << endl;

    return 0;
}
