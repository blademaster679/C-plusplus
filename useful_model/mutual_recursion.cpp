#include <iostream>

int expression_value(const char expr[], int &pos);

int factor_value(const char expr[], int &pos);

int term_value(const char expr[], int &pos);

int expression_value(const char expr[], int &pos)
{

    int result = term_value(expr, pos);

    while (expr[pos] == '+' || expr[pos] == '-')
    {

        char op = expr[pos++];

        int value = term_value(expr, pos);

        if (op == '+')
            result = result + value;

        else
            result = result - value;
    }

    return result;
}

int term_value(const char expr[], int &pos)
{

    int result = factor_value(expr, pos);

    while (expr[pos] == '*' || expr[pos] == '/')
    {

        char op = expr[pos++];

        int value = factor_value(expr, pos);

        if (op == '*')
            result = result * value;

        else
            result = result / value;
    }

    return result;
}

int factor_value(const char expr[], int &pos)
{

    int result = 0;

    if (expr[pos] == '(')
    {

        ++pos;

        result = expression_value(expr, pos);

        ++pos; // read ")"
    }

    else
    {

        while (expr[pos] >= '0' && expr[pos] <= '9')
        {

            result = 10 * result + expr[pos] - '0';

            ++pos;
        }
    }

    return result;
}

int main()
{

    // Note that no blank space is allowed

    int pos = 0;

    std::cout << expression_value("3+4*5", pos) << std::endl;

    pos = 0;

    std::cout << expression_value("(3+4)*5", pos) << std::endl;

    pos = 0;

    std::cout << expression_value("(3+4)*(5+1)/2+3", pos) << std::endl;

    return 0;
}