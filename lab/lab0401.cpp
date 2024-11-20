#include <iostream>
#include <vector>
using namespace std;
#include <cmath>

vector<vector<int>> multiply(const vector<vector<int>> &matrix1,const vector<vector<int>> &matrix2){
    int N = matrix1.size();
    int M = matrix1[0].size();
    vector<vector<int>> results(N,vector<int>(M));
    for (int i = 0; i< N;i ++){
        for (int j =0; j< M;j++){
            results[i][j] = matrix1[i][j] * matrix2[i][j];
        }
    }
    return results;
}

int main()
{
    int M , N;
    cin >> M >> N;
    vector<vector<int>> matrix1(M,vector<int>(N));
    vector<vector<int>> matrix2(M,vector<int>(N));
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int value = 0;
            cin >> value;
            matrix1[i][j] = value;
        }
    }
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int value = 0;
            cin >> value;
            matrix2[i][j] = value;
        }
    }
    vector<vector<int>> matrix3 = multiply(matrix1,matrix2);
    for (int i =0; i <M ;i++){
        for (int j =0 ;j <N ;j++){
            cout << matrix3[i][j] <<" ";
        }
        cout << "\n";
    }
}