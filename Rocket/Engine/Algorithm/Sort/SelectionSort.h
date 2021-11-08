#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap

namespace Rocket {
    // In-place Sort Big First
    template <typename T>
    void SelectionSortBF(T* data, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        for (int64_t i = 0; i < size; i++) {
            int64_t minIndex = i;
            for (int64_t j = i; j < size; j++) {
                if (compare(data[j], data[minIndex]) > 0)
                    minIndex = j;
            }
            std::swap(data[i], data[minIndex]);
        }
    }

    // In-place Sort Small First
    template <typename T>
    void SelectionSortSF(T* data, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        for (int64_t i = 0; i < size; i++) {
            int64_t minIndex = i;
            for (int64_t j = i; j < size; j++) {
                if (compare(data[j], data[minIndex]) < 0)
                    minIndex = j;
            }
            std::swap(data[i], data[minIndex]);
        }
    }
}
