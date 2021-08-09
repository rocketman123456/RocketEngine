#pragma once
#include <utility>
#include <memory>
#include <cstdint>

// Just For Int Sort
namespace Rocket {
    int32_t MaxBit(int32_t* data, std::size_t size) {
        int32_t d = 1;
        int32_t p = 10;
        for(int i = 0; i < size; ++i) {
            while(data[i] >= p) {
                p *= 10;
                ++d;
            }
        }
        return d;
    }

    void RadixSortBF(int32_t* array, std::size_t size) {
        if (size < 2) return;
        auto max_bit = MaxBit(array, size);
        int32_t* tmp = new int32_t[size];
        int32_t count[10];
        int32_t radix = 1;
        for(int32_t i = 1; i <= max_bit; i++) {
            for(int32_t j = 0; j < 10; j++)
                count[j] = 0;
            for(int32_t j = 0; j < size; j++) {
                auto k = (array[j] / radix) % 10;
                count[9 - k]++;
            }
            for(int32_t j = 1; j < 10; j++)
                count[j] = count[j - 1] + count[j];
            for(int32_t j = size - 1; j >= 0; j--) {
                auto k = (array[j] / radix) % 10;
                tmp[count[9 - k] - 1] = array[j];
                count[9 - k]--;
            }
            memcpy(array, tmp, size * sizeof(int32_t));
            radix = radix * 10;
        }
        delete[] tmp;
    }

    void RadixSortSF(int32_t* array, std::size_t size) {
        if (size < 2) return;
        auto max_bit = MaxBit(array, size);
        int32_t* tmp = new int32_t[size];
        int32_t count[10];
        int32_t radix = 1;
        for(int32_t i = 1; i <= max_bit; i++) {
            for(int32_t j = 0; j < 10; j++)
                count[j] = 0;
            for(int32_t j = 0; j < size; j++) {
                int32_t k = (array[j] / radix) % 10;
                count[k]++;
            }
            for(int32_t j = 1; j < 10; j++)
                count[j] = count[j - 1] + count[j]; 
            for(int32_t j = size - 1; j >= 0; j--) {
                int32_t k = (array[j] / radix) % 10;
                tmp[count[k] - 1] = array[j];
                count[k]--;
            }
            memcpy(array, tmp, size * sizeof(int32_t));
            radix = radix * 10;
        }
        delete[] tmp;
    }
}
