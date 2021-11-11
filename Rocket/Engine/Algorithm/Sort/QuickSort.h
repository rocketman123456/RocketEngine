#pragma once
#include <functional> // std::function<bool(T&, T&)>
#include <utility>    // std::swap
#include <random>
#include <exception>
#include <stdexcept>

#include "Algorithm/Random.h"

namespace Rocket {
    template<typename T>
    int64_t PartitionBF(T* array, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
        int64_t pivot = (int64_t) (start + r * (end - start + 1));
        int64_t bigIndex = start - 1;
        std::swap(array[pivot], array[end]);
        for (int64_t i = start; i <= end; i++) {
            if (compare(array[i], array[end]) >= 0) {
                bigIndex++;
                if (i > bigIndex) {
                    std::swap(array[i], array[bigIndex]);
                }
            }
        }
        return bigIndex;
    }

    template <typename T>
    void QuickSortBF(T* array, std::size_t size, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        if (start < 0 || end >= size || start > end) return;
        int64_t bigIndex = PartitionBF(array, start, end, compare);
        if (bigIndex > start)
            QuickSortBF(array, size, start, bigIndex - 1, compare);
        if (bigIndex < end)
            QuickSortBF(array, size, bigIndex + 1, end, compare);
    }

    // In-place Sort Big First
    template <typename T>
    void QuickSortBF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        QuickSortBF(array, size, 0, size-1, compare);
    }

    template<typename T>
    int64_t PartitionSF(T* array, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
        int64_t pivot = (int64_t) (start + r * (end - start + 1));
        int64_t smallIndex = start - 1;
        std::swap(array[pivot], array[end]);
        for (int64_t i = start; i <= end; i++) {
            if (compare(array[i], array[end]) <= 0) {
                smallIndex++;
                if (i > smallIndex) {
                    std::swap(array[i], array[smallIndex]);
                }
            }
        }
        return smallIndex;
    }

    template <typename T>
    void QuickSortSF(T* array, std::size_t size, int64_t start, int64_t end, std::function<int64_t(const T&,const T&)> compare) {
        if (start < 0 || end >= size || start > end) return;
        int64_t smallIndex = PartitionSF(array, start, end, compare);
        if (smallIndex > start)
            QuickSortSF(array, size, start, smallIndex - 1, compare);
        if (smallIndex < end)
            QuickSortSF(array, size, smallIndex + 1, end, compare);
    }

    // In-place Sort Small First
    template <typename T>
    void QuickSortSF(T* array, std::size_t size, std::function<int64_t(const T&,const T&)> compare) {
        if (size < 2) return;
        QuickSortSF(array, size, 0, size-1, compare);
    }

    template<typename It>
    class _QuickSF_ {
    public:
        void Sort(It beg, It end) {
            static Algorithm::Random r;
            r.Shuffle(beg, end);//随机化，消除对输入的依赖
            _Sort_(beg, end-1);
        }
    private:
        void _Sort_(It beg, It back) {
            if (back-beg <= 0) return;
            auto j = Partition(beg, back);
            if (j != beg)
                _Sort_(beg, j - 1);
            if (j != back)
                _Sort_(j + 1, back);
        }

        // 注意此技法
        // while(true){while(something){if(something) return/break;} 
        // 注意双层嵌套循环的终止条件和安排
        It Partition(It beg, It back) {
            It i = beg;
            It j = back+1;//左右扫描指针,注意跟下面的递增递减相统一
            auto v = *beg;//切分元素
            
            //循环不变式是：i左侧的元素都不大于v，j右侧的元素都不小于v
            while (true) {
                //扫描元素
                while (*++i < v) {
                    if (i == back) 
                        break;
                }
                while (v < *--j) {
                    if (j == beg) 
                        break;
                }
                //当指针i和j相遇时退出主循环
                if (i-j >= 0) 
                    break;
                std::swap(*i, *j);
            }
            //有*j不大于v，而*i不小于v，根据184页图例
            std::swap(*beg, *j);
            return j;
        }
    };
    
    template<typename It>
    void QuickSortSF(It beg, It end) {
        //static是为了random
        static _QuickSF_<It> q;
        q.Sort(beg, end);
    }

    template<typename It>
    class _QuickBF_ {
    public:
        void Sort(It beg, It end) {
            static Algorithm::Random r;
            r.Shuffle(beg, end);//随机化，消除对输入的依赖
            _Sort_(beg, end-1);
        }
    private:
        void _Sort_(It beg, It back) {
            if (back-beg <= 0) return;
            auto j = Partition(beg, back);
            if (j != beg)
                _Sort_(beg, j - 1);
            if (j != back)
                _Sort_(j + 1, back);
        }

        // 注意此技法
        // while(true){while(something){if(something) return/break;} 
        // 注意双层嵌套循环的终止条件和安排
        It Partition(It beg, It back) {
            It i = beg;
            It j = back+1;//左右扫描指针,注意跟下面的递增递减相统一
            auto v = *beg;//切分元素
            
            //循环不变式是：i左侧的元素都不大于v，j右侧的元素都不小于v
            while (true) {
                //扫描元素
                while (*++i > v) {
                    if (i == back) 
                        break;
                }
                while (v > *--j) {
                    if (j == beg) 
                        break;
                }
                //当指针i和j相遇时退出主循环
                if (i-j >= 0) 
                    break;
                std::swap(*i, *j);
            }
            //有*j不大于v，而*i不小于v，根据184页图例
            std::swap(*beg, *j);
            return j;
        }
    };
    
    template<typename It>
    void QuickSortBF(It beg, It end) {
        //static是为了random
        static _QuickBF_<It> q;
        q.Sort(beg, end);
    }

    template<typename It>
    class _Quick3WaySF_ {
    public:
        void Sort(It beg, It end) {
            static Algorithm::Random r;
            r.Shuffle(beg, end);//随机化，消除对输入的依赖
            _Sort_(beg, end - 1);
        }
    private:
        void _Sort_(It beg, It back) {
            if (back <= beg) return;
            It lt = beg, i = beg + 1, gt = back;
            auto v = *beg;
            //[beg,lt-1]的元素小于v,[lt..i-1]的元素等于v，[gt+1,back]的元素大于v，处理[i,gt]的元素
            //注意这种循环遍历方式与上面那种的不同
            while (i <= gt) {
                if (*i < v) std::swap(*lt++, *i++);
                else if (*i > v) std::swap(*i, *gt--);
                else  ++i;
            }
            _Sort_(beg, lt - 1);
            _Sort_(gt + 1, back);
        }
    
    };
    
    template<typename It>
    void Quick3WaySortSF(It beg, It end) {
        static _Quick3WaySF_<It> q;//static是为了random
        q.Sort(beg, end);
    }

    template<typename It>
    class _Quick3WayBF_ {
    public:
        void Sort(It beg, It end) {
            static Algorithm::Random r;
            r.Shuffle(beg, end);//随机化，消除对输入的依赖
            _Sort_(beg, end - 1);
        }
    private:
        void _Sort_(It beg, It back) {
            if (back <= beg) return;
            It lt = beg, i = beg + 1, gt = back;
            auto v = *beg;
            //[beg,lt-1]的元素小于v,[lt..i-1]的元素等于v，[gt+1,back]的元素大于v，处理[i,gt]的元素
            //注意这种循环遍历方式与上面那种的不同
            while (i <= gt) {
                if (*i > v) std::swap(*lt++, *i++);
                else if (*i < v) std::swap(*i, *gt--);
                else  ++i;
            }
            _Sort_(beg, lt - 1);
            _Sort_(gt + 1, back);
        }
    
    };
    
    template<typename It>
    void Quick3WaySortBF(It beg, It end) {
        static _Quick3WayBF_<It> q;//static是为了random
        q.Sort(beg, end);
    }
}
