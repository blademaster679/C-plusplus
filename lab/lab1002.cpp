#include <iostream>
#include <vector>
#include <deque>
using namespace std;

class Operation{
public:
    virtual void execute(vector<deque<int>>& rows) = 0;
    virtual ~Operation() = default;
};

class MoveOperation : public Operation{
private:
    int x,y;
public:
    MoveOperation(int x, int y) : x(x), y(y) {}
    void execute(vector<deque<int>>& rows){
        if(!rows[x].empty()){
            int student = rows[x].front();
            rows[x].pop_front();
            rows[y].push_back(student);
        }
    }
};

class QueryOperation : public Operation{
private:
    int x,y;
public:
    QueryOperation(int x, int y) : x(x), y(y) {}
    void execute(vector<deque<int>>& rows){
        if(!rows[x].empty() && y <= rows[x].size()){
            cout << rows[x][y-1] << endl;
        }else{
            cout << -1 << endl;
        }
    }
};

int main(){
    int N,M,Q;
    cin >> N >> M >> Q;
    vector<deque<int>> rows(N+1);
    for (int i = 1; i <= N ; i++){
        for(int j = 1; j <= M; j++){
            rows[i].push_back((i-1)*M+j);
        }
    }
    vector<Operation*> operations;
    while(Q--){
        int type,x,y;
        cin >> type >> x >> y;
        if(type == 1){
            operations.push_back(new MoveOperation(x,y));
        }else if(type == 2){
            operations.push_back(new QueryOperation(x,y));
        }
    }
    for (auto op:operations){
        op -> execute(rows);
    }
    for (auto op:operations){
        delete op;
    }
    return 0;
}