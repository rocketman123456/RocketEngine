#pragma once
#include <utility>
#include <memory>

// Just For Int Sort
namespace Rocket {
    void CountingSortBF(int32_t* array, std::size_t size) {
        if (size < 2) return;
        int32_t bias, min = array[0], max = array[0];
        for (int i = 1; i < size; i++) {
            if (array[i] > max)
                max = array[i];
            if (array[i] < min)
                min = array[i];
        }
        bias = 0 - min;
        int32_t* bucket = new int32_t[max - min + 1];
        memset(bucket, 0, sizeof(int32_t) * (max - min - 1));
        for (int i = 0; i < size; i++) {
            bucket[array[i] + bias]++;
        }
        int index = size-1, i = 0;
        while (index >= 0) {
            if (bucket[i] != 0) {
                array[index] = i - bias;
                bucket[i]--;
                index--;
            } else {
                i++;
            }
        }
        delete[] bucket;
    }

    void CountingSortSF(int32_t* array, std::size_t size) {
        if (size < 2) return;
        int32_t bias, min = array[0], max = array[0];
        for (int i = 1; i < size; i++) {
            if (array[i] > max)
                max = array[i];
            if (array[i] < min)
                min = array[i];
        }
        bias = 0 - min;
        int32_t* bucket = new int32_t[max - min + 1];
        memset(bucket, 0, sizeof(int32_t) * (max - min - 1));
        for (int i = 0; i < size; i++) {
            bucket[array[i] + bias]++;
        }
        int index = 0, i = 0;
        while (index < size) {
            if (bucket[i] != 0) {
                array[index] = i - bias;
                bucket[i]--;
                index++;
            } else {
                i++;
            }
        }
        delete[] bucket;
    }
}
