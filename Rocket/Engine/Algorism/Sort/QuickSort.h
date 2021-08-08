#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap
#include <random>

namespace Rocket {
    template<typename T>
    int64_t PartitionBF(T* array, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
        int64_t pivot = (int64_t) (start + r * (end - start + 1));
        int64_t bigIndex = start - 1;
        std::swap(array[pivot], array[end]);
        for (int64_t i = start; i <= end; i++) {
            if (compare(array[i], array[end]) >= 0) {
                bigIndex++;
                if (i > bigIndex) {
                    std::swap(array[i], array[bigIndex]);
                }
            }
        }
        return bigIndex;
    }

    template <typename T>
    void QuickSortBF(T* array, std::size_t size, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        if (start < 0 || end >= size || start > end) return;
        int64_t bigIndex = PartitionBF(array, start, end, compare);
        if (bigIndex > start)
            QuickSortBF(array, size, start, bigIndex - 1, compare);
        if (bigIndex < end)
            QuickSortBF(array, size, bigIndex + 1, end, compare);
    }

    // In-place Sort Big First
    template <typename T>
    void QuickSortBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        QuickSortBF(array, size, 0, size-1, compare);
    }

    template<typename T>
    int64_t PartitionSF(T* array, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
        int64_t pivot = (int64_t) (start + r * (end - start + 1));
        int64_t smallIndex = start - 1;
        std::swap(array[pivot], array[end]);
        for (int64_t i = start; i <= end; i++) {
            if (compare(array[i], array[end]) <= 0) {
                smallIndex++;
                if (i > smallIndex) {
                    std::swap(array[i], array[smallIndex]);
                }
            }
        }
        return smallIndex;
    }

    template <typename T>
    void QuickSortSF(T* array, std::size_t size, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        if (start < 0 || end >= size || start > end) return;
        int64_t smallIndex = PartitionSF(array, start, end, compare);
        if (smallIndex > start)
            QuickSortSF(array, size, start, smallIndex - 1, compare);
        if (smallIndex < end)
            QuickSortSF(array, size, smallIndex + 1, end, compare);
    }

    // In-place Sort Small First
    template <typename T>
    void QuickSortSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        QuickSortSF(array, size, 0, size-1, compare);
    }
}
