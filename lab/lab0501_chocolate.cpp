#include <iostream>
using namespace std;

int new_chocolate(int wrappers, int wrap){
    if (wrappers < wrap) return 0;
    return wrappers/wrap + new_chocolate(wrappers/wrap + wrappers%wrap,wrap);
}

int main(){
    int m,p,k;
    cin >> m >> p >> k;
    int chocolate = m/p + new_chocolate(m/p,k);
    cout << chocolate << endl;
}