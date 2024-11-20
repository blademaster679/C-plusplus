#include <iostream>
#include <vector>
using namespace std;

void polynomial(const vector<int> &result)
{
    bool first = true; // 标记是否为首项
    int degree = result.size() - 1;

    // 从高次幂开始遍历多项式
    for (int i = 0; i <= degree; i++)
    {
        int a = result[i];
        int power = degree - i;

        // 如果系数为 0，则跳过
        if (a == 0)
        {
            continue;
        }

        // 处理首项
        if (first)
        {
            if (power == 0)
            { // 常数项
                cout << a;
            }
            else if (a == 1)
            { // 系数为 1 时，省略 1
                cout << "x";
                if (power != 1)
                    cout << "^" << power;
            }
            else if (a == -1)
            { // 系数为 -1 时，省略 1
                cout << "-x";
                if (power != 1)
                    cout << "^" << power;
            }
            else
            { // 普通情况
                cout << a << "x";
                if (power != 1)
                    cout << "^" << power;
            }
            first = false; // 首项已处理完
        }
        else
        {
            // 处理后续项：根据正负决定是否添加符号
            if (a > 0)
            {
                cout << "+";
            }
            else
            {
                cout << "-";
                a = -a; // 变为正数以便输出
            }

            // 处理每项
            if (power == 0)
            { // 常数项
                cout << a;
            }
            else if (a == 1)
            { // 系数为 1 时，省略 1
                cout << "x";
                if (power != 1)
                    cout << "^" << power;
            }
            else
            { // 普通情况
                cout << a << "x";
                if (power != 1)
                    cout << "^" << power;
            }
        }
    }

    cout << endl;
}

vector<int> multiplypolynomials(const vector<int> &poly1, const vector<int> &poly2)
{
    int n = poly1.size();
    int m = poly2.size();

    // 结果多项式的长度应该是 n + m - 1
    vector<int> result(n + m - 1, 0);

    // 多项式相乘
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            result[i + j] += poly1[i] * poly2[j];
        }
    }

    return result;
}

int main()
{
    int d1, d2;
    cin >> d1;
    vector<int> poly1(d1 + 1);
    for (int i = 0; i <= d1; i++)
    {
        cin >> poly1[i];
    }
    cin >> d2;
    vector<int> poly2(d2 + 1);
    for (int j = 0; j <= d2; j++)
    {
        cin >> poly2[j];
    }
    vector<int> result = multiplypolynomials(poly1, poly2);
    polynomial(result);
}