#include "Memory/MemoryCheck.h"
#include "Algorithm/Sort/InsertionSort.h"

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

        InsertionSortSF<int32_t>(array, array_size, compare_function);

        std::cout << "InsertionSortSF Result: ";
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

        InsertionSortBF<int32_t>(array, array_size, compare_function);

        std::cout << "InsertionSortBF Result: ";
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

        InsertionSortSF(array.begin(), array.end());

        std::cout << "InsertionSortSF Result: ";
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

        InsertionSortBF(array.begin(), array.end());

        std::cout << "InsertionSortBF Result: ";
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

        InsertionSortXSF(array.begin(), array.end());

        std::cout << "InsertionSortXSF Result: ";
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

        InsertionSortXBF(array.begin(), array.end());

        std::cout << "InsertionSortXBF Result: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
