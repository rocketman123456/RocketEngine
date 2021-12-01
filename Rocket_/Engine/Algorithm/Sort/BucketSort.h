#pragma once
#include <utility>
#include <memory>

// Just For Int Sort
namespace Rocket {
    void BucketSortBF(int32_t* array, std::size_t size, std::size_t bucketSize) {
        if (size < 2) return;
        int32_t* bucket = new int32_t[bucketSize + 1];
        int count = 0;
        memset(bucket, 0, (bucketSize + 1) * sizeof(int32_t));
        for (int i = 0; i < size; i++) {
            auto j = array[i];
            bucket[j] += 1;
        }
        for (int i = bucketSize; i >= 0; i--) {
            if (bucket[i] > 0) {
                for (int j = 0; j < bucket[i]; j++) {
                    array[count] = i;
                    count++;
                }
            }
        }
        delete[] bucket;
    }

    void BucketSortSF(int32_t* array, std::size_t size, std::size_t bucketSize) {
        if (size < 2) return;
        int32_t* bucket = new int32_t[bucketSize + 1];
        int count = 0;
        memset(bucket, 0, (bucketSize + 1) * sizeof(int32_t));
        for (int i = 0; i < size; i++) {
            auto j = array[i];
            bucket[j] += 1;
        }
        for (int i = 0; i <= bucketSize; i++) {
            if (bucket[i] > 0) {
                for (int j = 0; j < bucket[i]; j++) {
                    array[count] = i;
                    count++;
                }
            }
        }
        delete[] bucket;
    }
}
