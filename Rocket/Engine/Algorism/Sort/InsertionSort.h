#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap
#include <cstring>    // std::memcpy

namespace Rocket {
    // In-place Sort Big First
    template <typename T>
    void InsertionSortBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        T current;
        for (int64_t i = 0; i < size - 1; i++) {
            std::memcpy(&current, &array[i + 1], sizeof(T));
            //current = array[i + 1];
            int64_t pre_index = i;
            while (pre_index >= 0 && (compare(current, array[pre_index]) > 0)) {
                array[pre_index + 1] = array[pre_index];
                pre_index--;
            }
            array[pre_index + 1] = current;
        }
    }

    // In-place Sort Small First
    template <typename T>
    void InsertionSortSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        T current;
        for (int64_t i = 0; i < size - 1; i++) {
            std::memcpy(&current, &array[i + 1], sizeof(T));
            //current = array[i + 1];
            int64_t pre_index = i;
            while (pre_index >= 0 && (compare(current, array[pre_index]) < 0)) {
                array[pre_index + 1] = array[pre_index];
                pre_index--;
            }
            array[pre_index + 1] = current;
        }
    }
}
