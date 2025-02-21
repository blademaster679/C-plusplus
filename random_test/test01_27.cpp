#include <stdio.h>
int main(){
    int a = 0;
    int b = 0;
    scanf("%d", &a);
    b = a/100 + a/10*10 -a/100*100 + a%10*100;
    printf("%d\n", b);
    return 0;
}