#pragma once
#include <functional>

namespace Rocket {
    namespace Algorism {
        // sorted  -- a sorted array, Small first !
        // compare -- >0 -> T1 > T2
        //         -- =0 -> T1 = T2
        //         -- <0 -> T1 < T2
        template<typename T>
        int64_t BinarySearchSF(T* sorted, std::size_t size, T& key, std::function<int64_t(const T&,const T&)> compare) {
            int64_t low = 0;
            int64_t high = static_cast<int64_t>(size - 1);
            while(low <= high) {
                int64_t mid = low + (high - low) / 2;
                int64_t result = compare(key, sorted[mid]);
                if(result < 0) high = mid - 1;
                else if(result > 0) low = mid + 1;
                else return mid;
            }
            // find nothing
            return -1;
        }
        // Big first
        template<typename T>
        int64_t BinarySearchBF(T* sorted, std::size_t size, T& key, std::function<int64_t(const T&,const T&)> compare) {
            int64_t low = 0;
            int64_t high = static_cast<int64_t>(size - 1);
            while(low <= high) {
                int64_t mid = low + (high - low) / 2;
                int64_t result = compare(key, sorted[mid]);
                if(result > 0) high = mid - 1;
                else if(result < 0) low = mid + 1;
                else return mid;
            }
            // find nothing
            return -1;
        }
    }
}