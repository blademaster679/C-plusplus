#include <iostream>

int** create_2D_array(int height, int width) {
    // 创建一个指向指针的数组，用于存储每一行的指针
    int** array = new int*[height];
    
    // 为每一行分配内存
    for (int i = 0; i < height; ++i) {
        array[i] = new int[width];
    }
    
    return array;
}

void populate_2D_array(int** array, int height, int width) {
    std::cout << "Enter values for the 2D array:\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cin >> array[i][j];
        }
    }
}

void print_2D_array(int** array, int height, int width) {
    std::cout << "2D array values:\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << array[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void delete_2D_array(int** array, int height) {
    // 释放每一行的内存
    for (int i = 0; i < height; ++i) {
        delete[] array[i];
    }
    // 释放行指针数组的内存
    delete[] array;
}

int main() {
    int height, width;
    
    std::cout << "Enter the number of rows (height): ";
    std::cin >> height;
    
    std::cout << "Enter the number of columns (width): ";
    std::cin >> width;

    // 创建二维数组
    int** array = create_2D_array(height, width);
    
    // 填充二维数组
    populate_2D_array(array, height, width);
    
    // 打印二维数组
    print_2D_array(array, height, width);
    
    // 释放内存
    delete_2D_array(array, height);

    return 0;
}
