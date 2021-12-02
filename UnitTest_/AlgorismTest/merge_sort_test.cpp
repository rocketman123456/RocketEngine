#include "Memory/MemoryCheck.h"
#include "Algorithm/Sort/MergeSort.h"

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

        MergeSortSF<int32_t>(array, array_size, compare_function);

        std::cout << "MergeSortSF Result: ";
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

        MergeSortBF<int32_t>(array, array_size, compare_function);

        std::cout << "MergeSortBF Result: ";
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

        MergeSortSF(array.begin(), array.end());

        std::cout << "MergeSortSF Result: ";
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

        MergeSortBF(array.begin(), array.end());

        std::cout << "MergeSortBF Result: ";
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

        MergeSortBUSF(array.begin(), array.end());

        std::cout << "MergeSortBUSF Result: ";
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

        MergeSortBUBF(array.begin(), array.end());

        std::cout << "MergeSortBUBF Result: ";
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

        MergeSortXSF(array.begin(), array.end());

        std::cout << "MergeSortXSF Result: ";
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

        MergeSortXBF(array.begin(), array.end());

        std::cout << "MergeSortXBF Result: ";
        for(int i = 0; i < array_size; ++i)
            std::cout << array[i] << ", ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return 0;
}