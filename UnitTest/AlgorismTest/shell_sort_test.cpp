
#include "Memory/MemoryCheck.h"
#include "Algorithm/Sort/ShellSort.h"

#include <vector>
#include <cstdint>
#include <random>
#include <iostream>

using namespace Rocket;

constexpr int32_t array_size = 10;

int main() {
    auto compare_function = [](const int32_t& k1, const int32_t& k2) -> int64_t { return k1 - k2; };
    {
        int32_t array[array_size] = {};
        for(int i = 0; i < array_size; ++i)
            array[i] = std::rand();
        std::cout << "Before Search: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;

        ShellSortSF<int32_t>(array, array_size, compare_function);

        std::cout << "ShellSortSF Result: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;
    }
    {
        int32_t array[array_size] = {};
        for(int i = 0; i < array_size; ++i)
            array[i] = std::rand();
        std::cout << "Before Search: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;

        ShellSortBF<int32_t>(array, array_size, compare_function);

        std::cout << "ShellSortBF Result: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    {
        std::vector<int32_t> array;
        array.resize(array_size);
        for(int i = 0; i < array_size; ++i)
            array[i] = std::rand();
        std::cout << "Before Search: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;

        ShellSortSF(array.begin(), array.end());

        std::cout << "ShellSortSF Result: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;
    }
    {
        std::vector<int32_t> array;
        array.resize(array_size);
        for(int i = 0; i < array_size; ++i)
            array[i] = std::rand();
        std::cout << "Before Search: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;

        ShellSortBF(array.begin(), array.end());

        std::cout << "ShellSortBF Result: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
