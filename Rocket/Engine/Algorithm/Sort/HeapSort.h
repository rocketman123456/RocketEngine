#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap

namespace Rocket {
    template <typename T>
    void AdjustHeapBF(T* array, std::size_t size, int64_t i, std::function<int64_t(const T&,const T&)> compare) {
        int64_t maxIndex = i;
        //������������������������ڸ��ڵ㣬�����ָ��ָ��������
        if (i * 2 < size && (compare(array[i * 2], array[maxIndex]) < 0))
            maxIndex = i * 2;
        //������������������������ڸ��ڵ㣬�����ָ��ָ��������
        if (i * 2 + 1 < size && (compare(array[i * 2 + 1], array[maxIndex]) < 0))
            maxIndex = i * 2 + 1;
        //������ڵ㲻�����ֵ���򽫸��ڵ������ֵ���������ҵݹ�����븸�ڵ㽻����λ�á�
        if (maxIndex != i) {
            std::swap(array[maxIndex], array[i]);
            AdjustHeapBF(array, size, maxIndex, compare);
        }
    }

    template <typename T>
    void BuildMaxHeapBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        for (int i = (size/2 - 1); i >= 0; i--) {
            AdjustHeapBF(array, size, i, compare);
        }
    }

    // In-place Sort Big First
    template <typename T>
    void HeapSortBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        BuildMaxHeapBF(array, size, compare);
        int64_t len = size;
        while (len > 0) {
            std::swap(array[0], array[len-1]);
            len--;
            AdjustHeapBF(array, len, 0, compare);
        }
    }

    template <typename T>
    void AdjustHeapSF(T* array, std::size_t size, int64_t i, std::function<int64_t(const T&,const T&)> compare) {
        int64_t maxIndex = i;
        //������������������������ڸ��ڵ㣬�����ָ��ָ��������
        if (i * 2 < size && (compare(array[i * 2], array[maxIndex]) > 0))
            maxIndex = i * 2;
        //������������������������ڸ��ڵ㣬�����ָ��ָ��������
        if (i * 2 + 1 < size && (compare(array[i * 2 + 1], array[maxIndex]) > 0))
            maxIndex = i * 2 + 1;
        //������ڵ㲻�����ֵ���򽫸��ڵ������ֵ���������ҵݹ�����븸�ڵ㽻����λ�á�
        if (maxIndex != i) {
            std::swap(array[maxIndex], array[i]);
            AdjustHeapSF(array, size, maxIndex, compare);
        }
    }

    template <typename T>
    void BuildMaxHeapSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        for (int i = (size/2 - 1); i >= 0; i--) {
            AdjustHeapSF(array, size, i, compare);
        }
    }

    // In-place Sort Small First
    template <typename T>
    void HeapSortSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        BuildMaxHeapSF(array, size, compare);
        int64_t len = size;
        while (len > 0) {
            std::swap(array[0], array[len-1]);
            len--;
            AdjustHeapSF(array, len, 0, compare);
        }
    }
}
