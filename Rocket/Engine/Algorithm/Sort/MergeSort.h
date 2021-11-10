#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap

#include "Algorithm/Sort/InsertionSort.h"

namespace Rocket {
    template <typename T>
    void MergeSF(T* A, T* L, int leftCount, T* R, int rightCount, std::function<int64_t(const T&,const T&)> compare) {
        int i,j,k;
    
        // i - to mark the index of left aubarray (L)
        // j - to mark the index of right sub-raay (R)
        // k - to mark the index of merged subarray (A)
        i = 0; j = 0; k =0;
    
        while(i<leftCount && j< rightCount) {
            if(compare(L[i], R[j]) < 0) A[k++] = L[i++];
            else A[k++] = R[j++];
        }
        while(i < leftCount) A[k++] = L[i++];
        while(j < rightCount) A[k++] = R[j++];
    }

    template <typename T>
    void MergeSortSF(T* A, int n, std::function<int64_t(const T&,const T&)> compare) {
        int mid,i, *L, *R;
        if(n < 2) return; // base condition. If the array has less than two element, do nothing.
    
        mid = n/2;  // find the mid index.
    
        // create left and right subarrays
        // mid elements (from index 0 till mid-1) should be part of left sub-array
        // and (n-mid) elements (from mid to n-1) will be part of right sub-array
        L = new T[mid];
        R = new T[n - mid];
    
        for(i = 0;i<mid;i++) L[i] = A[i]; // creating left subarray
        for(i = mid;i<n;i++) R[i-mid] = A[i]; // creating right subarray
    
        MergeSortSF(L,mid,compare);  // sorting the left subarray
        MergeSortSF(R,n-mid,compare);  // sorting the right subarray
        MergeSF(A,L,mid,R,n-mid,compare);  // Merging L and R into A as sorted list.
        // the delete operations is very important
        delete [] R;
        delete [] L;
    }

    template <typename T>
    void MergeBF(T* A, T* L, int leftCount, T* R, int rightCount, std::function<int64_t(const T&,const T&)> compare) {
        int i,j,k;
    
        // i - to mark the index of left aubarray (L)
        // j - to mark the index of right sub-raay (R)
        // k - to mark the index of merged subarray (A)
        i = 0; j = 0; k =0;
    
        while(i<leftCount && j< rightCount) {
            if(compare(L[i], R[j]) > 0) A[k++] = L[i++];
            else A[k++] = R[j++];
        }
        while(i < leftCount) A[k++] = L[i++];
        while(j < rightCount) A[k++] = R[j++];
    }

    template <typename T>
    void MergeSortBF(T* A, int n, std::function<int64_t(const T&,const T&)> compare) {
        int mid,i, *L, *R;
        if(n < 2) return; // base condition. If the array has less than two element, do nothing.
    
        mid = n/2;  // find the mid index.
    
        // create left and right subarrays
        // mid elements (from index 0 till mid-1) should be part of left sub-array
        // and (n-mid) elements (from mid to n-1) will be part of right sub-array
        L = new T[mid];
        R = new T[n - mid];
    
        for(i = 0; i < mid; i++) L[i] = A[i]; // creating left subarray
        for(i = mid; i < n; i++) R[i-mid] = A[i]; // creating right subarray
    
        MergeSortBF(L,mid,compare);  // sorting the left subarray
        MergeSortBF(R,n-mid,compare);  // sorting the right subarray
        MergeBF(A,L,mid,R,n-mid,compare);  // Merging L and R into A as sorted list.
        // the delete operations is very important
        delete [] R;
        delete [] L;
    }

    template<typename RandomAcessedIt>
    class _MergeSF_ {
        //typedef typename std::remove_reference<decltype(*beg)>::type value_type;
        typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;

    private:
        std::vector<value_type> aux;

    public:
        _MergeSF_(RandomAcessedIt first, RandomAcessedIt last) : aux(first, last) {
            Sort(first, last - 1);
        }

    private:
        void Sort(RandomAcessedIt lo, RandomAcessedIt hi) {
            //将数组[lo..hi]排序
            if (hi <= lo) return;
            auto mid = lo + (hi - lo) / 2;
            Sort(lo, mid);
            Sort(mid + 1, hi);
            Merge(lo, mid, hi);
        }

        void Merge(RandomAcessedIt lo , RandomAcessedIt mid, RandomAcessedIt hi) {
            //归并[lo mid] 和[mid+1 hi]
            auto i = lo, j = mid+1;
            for (int k = 0; k <= (hi-lo); ++k) {
                if (i > mid) aux[k] = std::move(*j++);
                else if (j > hi) aux[k] = std::move(*i++);
                else if (*i < *j) aux[k] = std::move(*i++);
                else aux[k] = std::move(*j++);
            }

            i = lo;
            for (int k = 0; k <= (hi - lo); ++k, ++i)
                *i = std::move(aux[k]);	
        }
    };

    template<typename RandomAcessedIt>
    void MergeSortSF(RandomAcessedIt beg, RandomAcessedIt end) {
        _MergeSF_<RandomAcessedIt>(beg, end);
    }

    template<typename RandomAcessedIt>
    class _MergeBF_ {
        //typedef typename std::remove_reference<decltype(*beg)>::type value_type;
        typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;

    private:
        std::vector<value_type> aux;

    public:
        _MergeBF_(RandomAcessedIt first, RandomAcessedIt last) : aux(first, last) {
            Sort(first, last - 1);
        }

    private:
        void Sort(RandomAcessedIt lo, RandomAcessedIt hi) {
            //将数组[lo..hi]排序
            if (hi <= lo) return;
            auto mid = lo + (hi - lo) / 2;
            Sort(lo, mid);
            Sort(mid + 1, hi);
            Merge(lo, mid, hi);
        }

        void Merge(RandomAcessedIt lo , RandomAcessedIt mid, RandomAcessedIt hi) {
            //归并[lo mid] 和[mid+1 hi]
            auto i = lo, j = mid+1;
            for (int k = 0; k <= (hi-lo); ++k) {
                if (i > mid) aux[k] = std::move(*j++);
                else if (j > hi) aux[k] = std::move(*i++);
                else if (*i > *j) aux[k] = std::move(*i++);
                else aux[k] = std::move(*j++);
            }

            i = lo;
            for (int k = 0; k <= (hi - lo); ++k, ++i)
                *i = std::move(aux[k]);	
        }
    };

    template<typename RandomAcessedIt>
    void MergeSortBF(RandomAcessedIt beg, RandomAcessedIt end) {
        _MergeBF_<RandomAcessedIt>(beg, end);
    }
}
