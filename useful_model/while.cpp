#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    int num = rand()%101;
    int val = 0;
    std::cout << "请输入一个1-100的整数" << std::endl;
    //std::cout << num << std::endl;
    int times = 0;
    bool guess = false;
    while(times < 6)
    {
        std::cin >> val;
        if (val>num)
        {
            std::cout << "猜测过大" << std::endl;
            times++;
        }
        else if (val<num)
        {
            std::cout << "猜测过小" << std::endl;
            times++;
        }
        else 
        {
            std::cout << "恭喜您，猜对了" << std::endl;
            guess = true;
            break;
        }
    }
    if (!guess)
    {
        std::cout << "很遗憾，游戏结束" << std::endl;
    }
    
return 0;
}