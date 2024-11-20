#include <iostream>
#include <vector>
using namespace std;

const int A = 101;

bool Magicsquare(vector<vector<int>> &matrix, int n, int sum)
{
    bool used[101] = {false};
    for (int i = 0; i < n; i++)
    {
        int rowsum = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] < 0 || matrix[i][j] > 100)
            {
                return false;
            }
            if (used[matrix[i][j]])
                return false;
            used[matrix[i][j]] = true;
            rowsum += matrix[i][j];
        }
        if (rowsum != sum)
            return false;
    }

    for (int y = 0; y < n; y++)
    {
        int colsum = 0;
        for (int x = 0; x < n; x++)
        {
            colsum += matrix[x][y];
        }
        if (colsum != sum)
            return false;
    }

    int diag1 = 0, diag2 = 0;
    for (int m = 0; m < n; m++)
    {
        diag1 += matrix[m][m];
        diag2 += matrix[m][n - m - 1];
    }
    if (diag1 != sum || diag2 != sum)
        return false;

    return true;
}

int sum(vector<vector<int>> &matrix, int n)
{
    int sum = -1;
    for (int i = 0; i < n; i++)
    {
        int rowsum = 0, countmissing = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == A)
            {
                countmissing++;
            }
            else
            {
                rowsum += matrix[i][j];
            }
        }
        if (countmissing == 0)
        {
            sum = rowsum;
            break;
        }
    }

    if (sum == -1)
    {
        for (int j = 0; j < n; j++)
        {
            int colsum = 0, countmissing = 0;
            for (int i = 0; i < n; i++)
            {
                if (matrix[i][j] == A)
                {
                    countmissing++;
                }
                else
                {
                    colsum += matrix[i][j];
                }
            }
            if (countmissing == 0)
            {
                sum = colsum;
                break;
            }
        }
    }

    if (sum == -1)
    {
        int diag1 = 0, countmissingdiag1 = 0, diag2 = 0, countmissingdiag2 = 0;
        for (int i = 0; i < n; i++)
        {
            if (matrix[i][i] == A)
            {
                countmissingdiag1++;
            }
            else
            {
                diag1 += matrix[i][i];
            }
            if (matrix[i][n - i - 1] == A)
            {
                countmissingdiag2++;
            }
            else
            {
                diag2 += matrix[i][n - i - 1];
            }
        }
        if (countmissingdiag1 == 0)
        {
            sum = diag1;
        }
        else if (countmissingdiag2 == 0)
        {
            sum = diag2;
        }
    }

    if (sum == -1 && n == 3)
    {
        return sum = 15;
    }
    return sum;
}

int main()
{
    int n;
    cin >> n;
    vector<vector<int>> matrix(n, vector<int>(n));
    int missingcount = 0;
    vector<pair<int, int>> missingpositions;
    bool used[101] = {false};
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            string s;
            cin >> s;
            if (s == "_")
            {
                matrix[i][j] = A;
                missingcount++;
                missingpositions.push_back({i, j});
            }
            else
            {
                matrix[i][j] = stoi(s);
                used[matrix[i][j]] = true;
            }
        }
    }
    if (missingcount != 3)
    {
        cout << "No solution" << endl;
        return 0;
    }
    int magicsum = sum(matrix, n);
    if (magicsum == -1)
    {
        cout << "No solution" << endl;
        return 0;
    }

    bool solutionFound = false;
    while (!solutionFound)
    {
        for (int a = 1; a <= 100; a++)
        {
            if (used[a])
                continue;
            for (int b = 1; b <= 100; b++)
            {
                if (a == b || used[b])
                    continue;
                for (int c = 1; c <= 100; c++)
                {
                    if (a == c || b == c || used[c])
                        continue;
                    matrix[missingpositions[0].first][missingpositions[0].second] = a;
                    matrix[missingpositions[1].first][missingpositions[1].second] = b;
                    matrix[missingpositions[2].first][missingpositions[2].second] = c;
                    if (Magicsquare(matrix, n, magicsum))
                    {
                        for (int i = 0; i < n; i++)
                        {
                            for (int j = 0; j < n; j++)
                            {
                                cout << matrix[i][j] << " ";
                            }
                            cout << endl;
                        }
                        solutionFound = true;
                        break;
                    }
                    matrix[missingpositions[0].first][missingpositions[0].second] = A;
                    matrix[missingpositions[1].first][missingpositions[1].second] = A;
                    matrix[missingpositions[2].first][missingpositions[2].second] = A;
                }
                if (solutionFound)
                    break;
            }
            if (solutionFound)
                break;
        }
        if (!solutionFound)
        {
            cout << "No solution" << endl;
            break;
        }
    }
    return 0;
}