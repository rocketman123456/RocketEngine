#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap

namespace Rocket {
    template<typename T>
    std::pair<T*, std::size_t> MergeBF(std::pair<T*, std::size_t> left, std::pair<T*, std::size_t> right, std::function<int64_t(const T&,const T&)> compare) {
        int64_t total_size = left.second + right.second;
        T* result = new T[total_size];
        for (int64_t index = 0, i = 0, j = 0; index < total_size; index++) {
            if (i >= left.second)
                result[index] = right.first[j++];
            else if (j >= right.second)
                result[index] = left.first[i++];
            else if (compare(left.first[i], right.first[j]) < 0)
                result[index] = right.first[j++];
            else
                result[index] = left.first[i++];
        }
        //delete[] left.first;
        //delete[] right.first;
        return {result, total_size};
    }

    // Out-place Sort Big First
    template <typename T>
    std::pair<T*, std::size_t> MergeSortBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return {array, size};
        int64_t mid = size / 2;
        T* left = new T[mid];
        memcpy(left, &array[0], sizeof(T) * mid);
        T* right = new T[size - mid];
        memcpy(right, &array[mid], sizeof(T) * (size - mid));
        auto left_result = MergeSortBF<T>(left, mid, compare);
        auto right_result = MergeSortBF<T>(right, size - mid, compare);
        auto result = MergeBF<T>(left_result, right_result, compare);
        delete[] left;
        delete[] right;
        delete[] left_result.first;
        delete[] right_result.first;
        return result;
    }

    template<typename T>
    std::pair<T*, std::size_t> MergeSF(std::pair<T*, std::size_t> left, std::pair<T*, std::size_t> right, std::function<int64_t(const T&,const T&)> compare) {
        int64_t total_size = left.second + right.second;
        T* result = new T[total_size];
        for (int64_t index = 0, i = 0, j = 0; index < total_size; index++) {
            if (i >= left.second)
                result[index] = right.first[j++];
            else if (j >= right.second)
                result[index] = left.first[i++];
            else if (compare(left.first[i], right.first[j]) > 0)
                result[index] = right.first[j++];
            else
                result[index] = left.first[i++];
        }
        //delete[] left.first;
        //delete[] right.first;
        return {result, total_size};
    }

    // Out-place Sort Small First
    template <typename T>
    std::pair<T*, std::size_t> MergeSortSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return {array, size};
        int64_t mid = size / 2;
        T* left = new T[mid];
        memcpy(left, &array[0], sizeof(T) * mid);
        T* right = new T[size - mid];
        memcpy(right, &array[mid], sizeof(T) * (size - mid));
        auto left_result = MergeSortSF<T>(left, mid, compare);
        auto right_result = MergeSortSF<T>(right, size - mid, compare);
        auto result = MergeSF<T>(left_result, right_result, compare);
        delete[] left;
        delete[] right;
        delete[] left_result.first;
        delete[] right_result.first;
        return result;
    }
}
