#include <iostream>
using namespace std;

void hanoi(int n, char start,char intermediate, char end, int &count){
    if (n==1){
        count++;
        return;
    }
    hanoi(n-1,start,end,intermediate,count);
    count++;
    hanoi(n-1,intermediate,start,end,count);
}

void print_hanoi(int n, char start,char intermediate, char end){
    if (n==1){
        cout << "move disk 1 from rod " << start << " to rod " << end << endl;
        return;
    }
    print_hanoi(n-1,start,end,intermediate);
    cout << "move disk " << n << " from rod " << start << " to rod " << end << endl;
    print_hanoi(n-1,intermediate,start,end);
}

int main(){
    int n;
    cin >> n;
    int count = 0;
    hanoi(n,'1','2','3',count);
    cout << count << endl;
    print_hanoi(n,'1','2','3');
    return 0;
}