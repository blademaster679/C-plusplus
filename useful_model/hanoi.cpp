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
        cout << "Move disk 1 from rod " << start << " to rod " << end << endl;
        return;
    }
    print_hanoi(n-1,start,end,intermediate);
    cout << "Move disk " << n << " from rod " << start << " to rod " << end << endl;
    print_hanoi(n-1,intermediate,start,end);
}

int main(){
    int n;
    cout << "Enter the number of disks: ";
    cin >> n;
    int count = 0;
    hanoi(n,'A','B','C',count);
    cout << "Number of moves: " << count << endl;
    print_hanoi(n,'A','B','C');
    return 0;
}