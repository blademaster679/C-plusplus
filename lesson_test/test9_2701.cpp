#include <iostream>
#include <vector>

bool isprime(int num)
{
    if (num <= 1)
    {
        return false;
    }
    if (num == 2)
    {
        return true;
    }
    for (int i = 2; i < num; i++)
    {
        if (num % i == 0)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    int N;
    std::cin >> N;
    std::vector<int> numbers(N);
    for (int j = 0; j < N; j++)
    {
        std::cin >> numbers[j];
    }
    int primecount = 0;
    for (int a = 0; a < N; a++)
    {
        if (isprime(numbers[a])){
            primecount++;
        }
        
    }
    std::cout << primecount << std::endl;
}