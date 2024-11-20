#include <iostream>
#include <vector>

int main() {
std::vector<int> numbers = {1, 2, 3, 4, 5};

    for (auto num : numbers) {
        std::cout << num << " ";
    }

    return 0;
}