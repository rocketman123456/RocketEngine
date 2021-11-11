#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap
#include <algorithm>  // std::min

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

    // Bottom Up
    template<typename RandomAcessedIt>
    class _MergeBUSF_ {
        typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;
    private:
        std::vector<value_type> aux;
    public:
        _MergeBUSF_(RandomAcessedIt beg, RandomAcessedIt end) : aux(beg, end) {
            int N = end - beg;
            for (int sz = 1; sz < N; sz = sz + sz)//sz是子数组的大小
                for (int lo = 0; lo < N - sz; lo += sz + sz)
                    Merge(beg + lo, beg + lo + sz - 1, beg + std::min(lo + sz + sz - 1, N - 1));
        }
    private:
        void Merge(RandomAcessedIt lo, RandomAcessedIt mid, RandomAcessedIt hi);//使用2.2.10的快速归并
    };

    template<typename RandomAcessedIt>
    void _MergeBUSF_<RandomAcessedIt>::Merge(RandomAcessedIt lo, RandomAcessedIt mid, RandomAcessedIt hi) {
        int lsize = (mid - lo) + 1;
        int rsize = (hi - mid);

        for (int k = 0; k < lsize; ++k)
            aux[k] = std::move(*(lo + k));
        for (int k = 0; k < rsize; ++k)
            aux[lsize + rsize - 1 - k] = std::move(*(mid + k));

        auto i = aux.begin(), j = aux.begin() + (hi - lo);
        auto ret = lo;
        for (int k = 0; k < lsize + rsize; ++k, ++ret) {
            if (*i < *j) *ret = std::move(*i++);
            else *ret = std::move(*j--);
        }
    }

    template<typename RandomAcessedIt>
    void MergeSortBUSF(RandomAcessedIt beg, RandomAcessedIt end) {
        _MergeBUSF_<RandomAcessedIt>(beg, end);
    }

    // 改进归并排序的算法,
    // 1.加快小数组的排序速度；2.检测数组是否已经有序；3.通过在递归中交换参数来避免数组复制
    template<typename RandomAcessedIt>
    class _MergeXSF_ {
        typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;
    private:
        std::vector<value_type> aux;
        static constexpr int32_t CUTOFF = 15;
    public:
        _MergeXSF_(RandomAcessedIt beg, RandomAcessedIt end) :aux(beg, end) {
            Sort( aux.begin(), aux.end() - 1, beg, end - 1);
        }
    private:
        void Sort(RandomAcessedIt srclo, RandomAcessedIt srchi, RandomAcessedIt destlo, RandomAcessedIt desthi) {
            //将数组[srclo,srchi]排序到[destlo,desthi],其中必须开始时，两个区间对应元素相等
            //小数组用插入排序提高速度
            if ((srchi - srclo) <= CUTOFF) {
                InsertionSortXSF(destlo, desthi + 1);
                return;
            }

            //
            auto srcmid = srclo + (srchi - srclo) / 2;
            auto destmid = destlo + (desthi - destlo) / 2;

            Sort(destlo, destmid, srclo, srcmid);
            Sort(destmid + 1, desthi, srcmid + 1, srchi);

            //检测是否已经有序
            if (*srcmid <= *(srcmid + 1)) {
                std::copy(srclo, srchi, destlo);
                return;
            }

            Merge(srclo, srcmid, srchi, destlo, desthi);
        }

        void Merge(RandomAcessedIt srclo, RandomAcessedIt srcmid, RandomAcessedIt srchi, RandomAcessedIt destlo, RandomAcessedIt desthi) {
            //将[srclo, srcmid,srchi]归并到[destlo,desthi],其中source已经有序
            auto i = srclo, j = srcmid + 1;
            auto dest = destlo;
            for (int k = 0; k <= (srchi - srclo); ++k, ++dest) {
                if (i > srcmid) *dest = std::move(*j++);
                else if (j > srchi) *dest = std::move(*i++);
                else if (*i < *j) *dest = std::move(*i++);
                else *dest = std::move(*j++);
            }
        }
    };

    template<typename RandomAcessedIt>
    void MergeSortXSF(RandomAcessedIt beg, RandomAcessedIt end) {
        _MergeXSF_<RandomAcessedIt>(beg, end);
    }

    // Bottom Up
    template<typename RandomAcessedIt>
    class _MergeBUBF_ {
        typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;
    private:
        std::vector<value_type> aux;
    public:
        _MergeBUBF_(RandomAcessedIt beg, RandomAcessedIt end) : aux(beg, end) {
            int N = end - beg;
            for (int sz = 1; sz < N; sz = sz + sz)//sz是子数组的大小
                for (int lo = 0; lo < N - sz; lo += sz + sz)
                    Merge(beg + lo, beg + lo + sz - 1, beg + std::min(lo + sz + sz - 1, N - 1));
        }
    private:
        void Merge(RandomAcessedIt lo, RandomAcessedIt mid, RandomAcessedIt hi);//使用2.2.10的快速归并
    };

    template<typename RandomAcessedIt>
    void _MergeBUBF_<RandomAcessedIt>::Merge(RandomAcessedIt lo, RandomAcessedIt mid, RandomAcessedIt hi) {
        int lsize = (mid - lo) + 1;
        int rsize = (hi - mid);

        for (int k = 0; k < lsize; ++k)
            aux[k] = std::move(*(lo + k));
        for (int k = 0; k < rsize; ++k)
            aux[lsize + rsize - 1 - k] = std::move(*(mid + k));

        auto i = aux.begin(), j = aux.begin() + (hi - lo);
        auto ret = lo;
        for (int k = 0; k < lsize + rsize; ++k, ++ret) {
            if (*i > *j) *ret = std::move(*i++);
            else *ret = std::move(*j--);
        }
    }

    template<typename RandomAcessedIt>
    void MergeSortBUBF(RandomAcessedIt beg, RandomAcessedIt end) {
        _MergeBUBF_<RandomAcessedIt>(beg, end);
    }

    // 改进归并排序的算法,
    // 1.加快小数组的排序速度；2.检测数组是否已经有序；3.通过在递归中交换参数来避免数组复制
    template<typename RandomAcessedIt>
    class _MergeXBF_ {
        typedef typename std::iterator_traits<RandomAcessedIt>::value_type value_type;
    private:
        std::vector<value_type> aux;
        static constexpr int32_t CUTOFF = 15;
    public:
        _MergeXBF_(RandomAcessedIt beg, RandomAcessedIt end) :aux(beg, end) {
            Sort( aux.begin(), aux.end() - 1, beg, end - 1);
        }
    private:
        void Sort(RandomAcessedIt srclo, RandomAcessedIt srchi, RandomAcessedIt destlo, RandomAcessedIt desthi) {
            //将数组[srclo,srchi]排序到[destlo,desthi],其中必须开始时，两个区间对应元素相等
            //小数组用插入排序提高速度
            if ((srchi - srclo) <= CUTOFF) {
                InsertionSortXSF(destlo, desthi + 1);
                return;
            }

            //
            auto srcmid = srclo + (srchi - srclo) / 2;
            auto destmid = destlo + (desthi - destlo) / 2;

            Sort(destlo, destmid, srclo, srcmid);
            Sort(destmid + 1, desthi, srcmid + 1, srchi);

            //检测是否已经有序
            if (*srcmid >= *(srcmid + 1)) {
                std::copy(srclo, srchi, destlo);
                return;
            }

            Merge(srclo, srcmid, srchi, destlo, desthi);
        }

        void Merge(RandomAcessedIt srclo, RandomAcessedIt srcmid, RandomAcessedIt srchi, RandomAcessedIt destlo, RandomAcessedIt desthi) {
            //将[srclo, srcmid,srchi]归并到[destlo,desthi],其中source已经有序
            auto i = srclo, j = srcmid + 1;
            auto dest = destlo;
            for (int k = 0; k <= (srchi - srclo); ++k, ++dest) {
                if (i > srcmid) *dest = std::move(*j++);
                else if (j > srchi) *dest = std::move(*i++);
                else if (*i > *j) *dest = std::move(*i++);
                else *dest = std::move(*j++);
            }

        }
    };

    template<typename RandomAcessedIt>
    void MergeSortXBF(RandomAcessedIt beg, RandomAcessedIt end) {
        _MergeXBF_<RandomAcessedIt>(beg, end);
    }
}
