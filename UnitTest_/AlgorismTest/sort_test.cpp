#include "Memory/MemoryCheck.h"

#include "Algorithm/Sort/MergeSort.h"
#include "Algorithm/Sort/QuickSort.h"
#include "Algorithm/Sort/HeapSort.h"
#include "Algorithm/Sort/CountingSort.h"
#include "Algorithm/Sort/BucketSort.h"
#include "Algorithm/Sort/RadixSort.h"

#include <vector>
#include <cstdint>
#include <random>
#include <iostream>

using namespace Rocket;

constexpr int32_t array_size = 10;

int main() {
    auto compare_function = [](const int32_t& k1, const int32_t& k2) -> int64_t { return k1 - k2; };
    std::cout << std::endl;
    {
        // int32_t array[array_size] = {};
        // for(int i = 0; i < array_size; ++i)
        //     array[i] = std::rand();
        // std::cout << "Before Search: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;

        // CountingSortSF(array, array_size);

        // std::cout << "CountingSortSF Result: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;
    }
    {
        // int32_t array[array_size] = {};
        // for(int i = 0; i < array_size; ++i)
        //     array[i] = std::rand();
        // std::cout << "Before Search: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;

        // CountingSortBF(array, array_size);

        // std::cout << "CountingSortBF Result: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;
    }
    std::cout << std::endl;
    {
        // int32_t array[array_size] = {};
        // for(int i = 0; i < array_size; ++i)
        //     array[i] = std::rand();
        // std::cout << "Before Search: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;

        // int32_t max = array[0];
        // for(int i = 0; i < array_size; ++i) {
        //     if(max < array[i])
        //         max = array[i];
        // }

        // BucketSortSF(array, array_size, max);

        // std::cout << "BucketSortSF Result: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;
    }
    {
        // int32_t array[array_size] = {};
        // for(int i = 0; i < array_size; ++i)
        //     array[i] = std::rand();
        // std::cout << "Before Search: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;

        // int32_t max = array[0];
        // for(int i = 0; i < array_size; ++i) {
        //     if(max < array[i])
        //         max = array[i];
        // }

        // BucketSortBF(array, array_size, max);

        // std::cout << "BucketSortBF Result: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;
    }
    std::cout << std::endl;
    {
        // int32_t array[array_size] = {};
        // for(int i = 0; i < array_size; ++i)
        //     array[i] = std::rand();
        // std::cout << "Before Search: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;

        // RadixSortSF(array, array_size);

        // std::cout << "RadixSortSF Result: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;
    }
    {
        // int32_t array[array_size] = {};
        // for(int i = 0; i < array_size; ++i)
        //     array[i] = std::rand();
        // std::cout << "Before Search: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;

        // RadixSortBF(array, array_size);

        // std::cout << "RadixSortBF Result: ";
        // for(int i = 0; i < array_size; ++i)
        //     std::cout << array[i] << ", ";
        // std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
