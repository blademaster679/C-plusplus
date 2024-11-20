#include <iostream>


int main()
{
    int a,b;
    int gcd =0;
    std::cin >> a >> b;
    for (int i=a;i>=1;--i){
        if ((a%i==0)&&(b%i==0)){
            std::cout << i << std::endl;
            break;
        }
    }
    
}
// bgfd;
// _GLIBCXX_HAVE_LOGF