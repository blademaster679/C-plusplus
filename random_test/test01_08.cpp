#include <iostream>

// int PRODUCT(int a, int b) {
//     return a * b;
// }

#define PRODUCT(a, b) ((a)*(b))

void fun(int n){
    static int x = 1;
    printf("%d ",x);
    printf("x=%d\n",x+n);
    x += PRODUCT(x+n, x-n);
}

int main(){
    int i, x=1;
    for(i=1; i<=3; i++, x++){
        fun(x+i);
    }
}