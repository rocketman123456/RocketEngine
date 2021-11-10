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

    template<typename BidirectionalIt>
    void SelectionSortSF(BidirectionalIt first, BidirectionalIt last) {
        typedef typename std::iterator_traits<BidirectionalIt>::value_type value_type;
        if (first == last) return;
        for (auto i = first; i != last; ++i) {
            //将a[i]与a[i+1..N)中的最小元素交换
            auto min = i;
            for (auto j = i + 1; j != last; ++j) {
                if (*j < *min) min = j;
            }
            std::swap(*i, *min);
        }
    }

    template<typename BidirectionalIt>
    void SelectionSortBF(BidirectionalIt first, BidirectionalIt last) {
        typedef typename std::iterator_traits<BidirectionalIt>::value_type value_type;
        if (first == last) return;
        for (auto i = first; i != last; ++i) {
            //将a[i]与a[i+1..N)中的最小元素交换
            auto max = i;
            for (auto j = i + 1; j != last; ++j) {
                if (*j > *max) max = j;
            }
            std::swap(*i, *max);
        }
    }
}
