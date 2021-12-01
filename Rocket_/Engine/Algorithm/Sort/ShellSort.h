#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap
#include <cstring>    // std::memcpy

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
                std::memcpy(&temp, &array[i], sizeof(T));
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
                std::memcpy(&temp, &array[i], sizeof(T));
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

    template<typename RandomAcessIt>
    void ShellSortSF(RandomAcessIt first, RandomAcessIt last) {
        int N = (last - first);
        int h = 1;
        //1, 4, 13, 40, 121, 364, 1093, ...
        while (h < N / 3) h = 3 * h + 1;
        while (h >= 1) { 
            //将数组变成h有序
            for (int i = h; i < N; ++i) {
                //将a[i]插入到a[i-h..i-2*h..]之中
                auto t = *(first + i);
                auto j = first + i;
                for (; j>=(first+h) && t < *(j - h); j-=h)
                    *j = std::move(*(j - h));
                *j = std::move(t);
            }
            h = h / 3;
        }
    }

    template<typename RandomAcessIt>
    void ShellSortBF(RandomAcessIt first, RandomAcessIt last) {
        int N = (last - first);
        int h = 1;
        //1, 4, 13, 40, 121, 364, 1093, ...
        while (h < N / 3) h = 3 * h + 1;
        while (h >= 1) { 
            //将数组变成h有序
            for (int i = h; i < N; ++i) {
                //将a[i]插入到a[i-h..i-2*h..]之中
                auto t = *(first + i);
                auto j = first + i;
                for (; j>=(first+h) && t > *(j - h); j-=h)
                    *j = std::move(*(j - h));
                *j = std::move(t);
            }
            h = h / 3;
        }
    }
}
