#include <iostream>
#include <vector>
using namespace std;
#include <cmath>

int limitvalue(int value) {
    if (abs(value) > 10000){
        return (value > 0) ? 10000 : -10000;
    }
    return value;
}

void print(vector<vector<int>>& matrix){
    for (int i = 0; i< matrix.size(); i ++){
        for (int j = 0; j< matrix[i].size() ; j++){
            cout << matrix[i][j]<<" ";
        }
        cout << "\n";
    }
}

vector<vector<int>> transposed(const vector<vector<int>>& matrix){
    int N = matrix.size();
    int M = matrix[0].size();
    vector<vector<int>> results(M,vector<int>(N));
    for (int i = 0; i< N ; i++){
        for (int j = 0 ; j< M ; j++){
            results[j][i] = matrix[i][j];
        }
    }
    return results;
}



int main(){
    int N , M;
    cin >> N >> M;
    vector<vector<int>> matrix(N,vector<int>(M));
    for(int i = 0 ; i < N ; i++){
        for (int j = 0 ; j< M ; j++){
            int value = 0;
            cin >> value;
            matrix[i][j] = limitvalue(value);
        }
    }
    vector<vector<int>> matrix2 = transposed(matrix);
    print(matrix2);
}



