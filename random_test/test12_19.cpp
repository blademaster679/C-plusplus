#include <iostream>
int i = 1;
int main()
{
    std::cout << "i++: " << i++ << std::endl; // i++ is a post-increment operator, so it will print the value of i first and then increment it.
    std::cout << "After i++: " << i << std::endl;
    i = 1;                                    // Reset i to 1
    std::cout << "++i: " << ++i << std::endl; // ++i is a pre-increment operator, so it will increment the value of i first and then print it.
    std::cout << "After ++i: " << i << std::endl;
    int num = 11;
    int &ref = num;
    ref = 15;
    std::cout << "num: " << num << std::endl; // num is a reference to ref, so changing ref will change num as well.
    std::cout << "ref: " << ref << std::endl;
}