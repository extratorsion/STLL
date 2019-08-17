#include <iostream>
#include <vector>

#include "memory_pool.h"

int main(void) {
    for (int j = 0; j < 100; ++j){
        std::vector<int, simple_alloc<int>> vec;
        for (int i = 0; i < 100; ++i)
            vec.push_back(i);
        for (int elem : vec)
            std::cout << elem << " ";
        std::cout << std::endl;
    }
}
