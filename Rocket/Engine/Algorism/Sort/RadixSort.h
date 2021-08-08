#pragma once
#include <utility>
#include <memory>

// Just For Int Sort
namespace Rocket {
    int32_t MaxBit(int32_t* data, std::size_t size) {
        int32_t d = 1;  //保存最大的位数
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
        int32_t count[10]; //计数器
        int32_t radix = 1;
        for(int i = 1; i <= max_bit; i++) { //进行d次排序
            for(int j = 0; j < 10; j++)
                count[j] = 0; //每次分配前清空计数器
            for(int j = 0; j < size; j++) {
                auto k = (array[j] / radix) % 10; //统计每个桶中的记录数
                count[9 - k]++;
            }
            for(int j = 1; j < 10; j++)
                count[j] = count[j - 1] + count[j]; //将tmp中的位置依次分配给每个桶
            for(int j = size - 1; j >= 0; j--) { //将所有桶中记录依次收集到tmp中
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
        int32_t count[10]; //计数器
        int32_t i, j, k;
        int32_t radix = 1;
        for(i = 1; i <= max_bit; i++) { //进行d次排序
            for(j = 0; j < 10; j++)
                count[j] = 0; //每次分配前清空计数器
            for(j = 0; j < size; j++) {
                k = (array[j] / radix) % 10; //统计每个桶中的记录数
                count[k]++;
            }
            for(j = 1; j < 10; j++)
                count[j] = count[j - 1] + count[j]; //将tmp中的位置依次分配给每个桶
            for(j = size - 1; j >= 0; j--) { //将所有桶中记录依次收集到tmp中
                k = (array[j] / radix) % 10;
                tmp[count[k] - 1] = array[j];
                count[k]--;
            }
            memcpy(array, tmp, size * sizeof(int32_t));
            radix = radix * 10;
        }
        delete[] tmp;
    }
}
