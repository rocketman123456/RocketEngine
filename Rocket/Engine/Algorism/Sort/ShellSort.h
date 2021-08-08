#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap

namespace Rocket {
    // In-place Sort Big First
    template <typename T>
    void ShellSortBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        int64_t len = size;
        int64_t gap = len / 2;
        T temp;
        while (gap > 0) {
            for (int64_t i = gap; i < len; i++) {
                memcpy(&temp, &array[i], sizeof(T));
                //temp = array[i];
                int64_t pre_index = i - gap;
                while (pre_index >= 0 && (compare(array[pre_index], temp) < 0)) {
                    array[pre_index + gap] = array[pre_index];
                    pre_index -= gap;
                }
                array[pre_index + gap] = temp;
            }
            gap /= 2;
        }
    }

    // In-place Sort Small First
    template <typename T>
    void ShellSortSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        int64_t len = size;
        int64_t gap = len / 2;
        T temp;
        while (gap > 0) {
            for (int64_t i = gap; i < len; i++) {
                memcpy(&temp, &array[i], sizeof(T));
                //temp = array[i];
                int64_t pre_index = i - gap;
                while (pre_index >= 0 && (compare(array[pre_index], temp) > 0)) {
                    array[pre_index + gap] = array[pre_index];
                    pre_index -= gap;
                }
                array[pre_index + gap] = temp;
            }
            gap /= 2;
        }
    }
}
