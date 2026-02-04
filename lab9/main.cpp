#include "PoolAllocator.h"
#include <list>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>

int main() {
    std::ofstream file("text.txt");
    for(int j = 1; j <= 10000; j++) {
        std::list<int, PoolAllocator<int, 800000, 100000>> vec;
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < j; i++)
            vec.push_back(i);
        auto end = std::chrono::steady_clock::now();
        file << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " " << j << "\n";
    }
    return 0;
}
