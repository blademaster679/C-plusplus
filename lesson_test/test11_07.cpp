#include <iostream>

void modify_value(int arr[]){
    arr[3] = 5;
}
int main(){
    int arr[]={1,2,3,4,5};
    modify_value(arr);
    for(int i=0; i<5; i++){
        std::cout << arr[i] << " ";
    }
} 