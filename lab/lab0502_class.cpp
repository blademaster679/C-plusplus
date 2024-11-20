#include <iostream>
using namespace std;
class Time{
    public:
        Time(){
            cout << "Time object created!" << endl;
            }
        int hh = 23;
        int mm = 59;
        int ss = 59;
};

int main() {

Time mytime;

cout << mytime.hh <<endl;

cout << mytime.mm <<endl;

cout << mytime.ss <<endl;

return 0;

}