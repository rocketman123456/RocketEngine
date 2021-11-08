#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap

namespace Rocket
{
    // In-place Sort Big First
    template <typename T>
    void BubbleSortBF(T* data, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        for (int64_t i = 0; i < size; i++) {
            for (int64_t j = size - 1; j > 0 + i; j--) {
                if (compare(data[j], data[j - 1]) > 0) {
                    std::swap(data[j], data[j - 1]);
                }
            }
        }
    }

    // In-place Sort Small First
    template <typename T>
    void BubbleSortSF(T* data, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        for (int64_t i = 0; i < size; i++) {
            for (int64_t j = 0; j < size - 1 - i; j++) {
                if (compare(data[j + 1], data[j]) < 0) {
                    std::swap(data[j + 1], data[j]);
                }
            }
        }
    }
}
