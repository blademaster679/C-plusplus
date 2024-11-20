#include <iostream>
#include <vector>
using namespace std;

void print(const vector<vector<int>> &vec){
    for (int i=0; i<vec.size(); i++){
        cout << "[";
        for (int j=0; j<vec[i].size(); j++){
            cout << vec[i][j] << " ";
        }
        cout << "]\n";
    }
}

void addeachpos(const vector<int> &vec ,int a,vector<vector<int>>& results){
    for(int i=0; i<=vec.size();i++){
        vector<int> temp =vec;
        temp.insert(temp.begin()+i,a);
        results.push_back(temp);
    }
}

vector<vector<int>> permute(vector<int> vec){
    vector<vector<int>> results;
    if(vec.size()==1){
        results.push_back(vec);
        return results;
    }
    int a = vec[vec.size()-1];
    vec.pop_back();
    vector<vector<int>> smaller = permute(vec);
    for (int i=0; i<smaller.size(); ++i){
        addeachpos(smaller[i],a,results);
    }
    return results;
}

int main(){
    vector<int> vec = {1,2,3,};
    vector<vector<int>> results = permute(vec);
    //vector<vector<int>> p = {{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}};
    print(results);
    return 0;
}