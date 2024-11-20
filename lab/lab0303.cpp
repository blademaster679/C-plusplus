#include <iostream>
#include <vector>
using namespace std;

bool found(const vector<int> &vec, int target)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == target)
        {
            return true;
        }
    }
    return false;
}

void sort(vector<int> &vec)
{
    int n = vec.size();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (vec[j] > vec[j + 1])
            {
                int temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
            }
        }
    }
}

vector<int> vector2(vector<int> &vec, int target)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == target)
        {
            vec.erase(vec.begin() + i);
            --i;
        }
    }
    return vec;
}

int main()
{
    vector<int> numbers;
    int num;

    while (true)
    {
        cin >> num;
        if (num == -1)
        {
            break;
        }
        numbers.push_back(num);
    }
int target;
cin >> target;
if (found(numbers, target))
{
    vector2(numbers, target);
    sort(numbers);
    for (int i = 0; i < numbers.size(); i++)
    {
        cout << numbers[i] << " ";
    }
}
else
{
    cout << "NOT FOUND" << endl;
}
}