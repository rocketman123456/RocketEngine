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

    template<typename BidirectionalIt>
    void InsertionSortSF(BidirectionalIt first, BidirectionalIt last) {
        typedef typename std::iterator_traits<BidirectionalIt>::value_type value_type;
        if (first == last) return;
        for (auto i = first + 1; i != last; ++i) {
            //将a[i]插入到a[0...i-1]之中
            for (auto j = i; j > first && *j < *(j - 1); --j) {
                std::swap(*j, *(j - 1));
            }
        }
    }

    template<typename BidirectionalIt>
    void InsertionSortBF(BidirectionalIt first, BidirectionalIt last) {
        typedef typename std::iterator_traits<BidirectionalIt>::value_type value_type;
        if (first == last) return;
        for (auto i = first + 1; i != last; ++i) {
            //将a[i]插入到a[0...i-1]之中
            for (auto j = i; j > first && *j > *(j - 1); --j) {
                std::swap(*j, *(j - 1));
            }
        }
    }

    template<typename BidirectionalIt>
    void InsertionSortXSF(BidirectionalIt first, BidirectionalIt last) {
        typedef typename std::iterator_traits<BidirectionalIt>::value_type value_type;

        if (first == last) return;
        // put smallest element in position to serve as sentinel
        int exchanges = 0;
        for (auto i = last - 1; i != first; --i)  {
            if (*i < *(i-1)) {
                std::swap(*i, *(i - 1));
                ++exchanges;
            }
        }
        if (exchanges == 0) return;

        //不需要交换的插入排序,将赋值操作减半
        //insertion sort with half-exchanges
        for (auto i = (first + 1) + 1; i != last; ++i) {
            //将a[i]插入到a[0...i-1]之中
            auto t = *i;
            auto j = i;
            for (; t < *(j - 1); --j) {
                *j = std::move(*(j - 1));
            }
            *j = std::move(t);
        }
    }

    template<typename BidirectionalIt>
    void InsertionSortXBF(BidirectionalIt first, BidirectionalIt last) {
        typedef typename std::iterator_traits<BidirectionalIt>::value_type value_type;

        if (first == last) return;
        // put smallest element in position to serve as sentinel
        int exchanges = 0;
        for (auto i = last - 1; i != first; --i)  {
            if (*i > *(i-1)) {
                std::swap(*i, *(i - 1));
                ++exchanges;
            }
        }
        if (exchanges == 0) return;

        //不需要交换的插入排序,将赋值操作减半
        //insertion sort with half-exchanges
        for (auto i = (first + 1) + 1; i != last; ++i) {
            //将a[i]插入到a[0...i-1]之中
            auto t = *i;
            auto j = i;
            for (; t > *(j - 1); --j) {
                *j = std::move(*(j - 1));
            }
            *j = std::move(t);
        }
    }
}
