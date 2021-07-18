#pragma once
#include <functional>

namespace Rocket {
    namespace Algorism {
        // sorted  -- a sorted array, Small first !
        // compare -- >0 T1 > T2
        //         -- =0 T1 = T2
        //         -- <0 T1 < T2
        template<typename T>
        int32_t BinarySearchSF(T* sorted, std::size_t size, T& key, std::function<int32_t(T&,T&)> compare) {
            int32_t low = 0;
            int32_t high = size - 1;
            while(low <= high) {
                int32_t mid = low + (high - low) / 2;
                int32_t result = compare(key, sorted[mid]);
                if(result < 0) high = mid - 1;
                else if(result > 0) low = mid + 1;
                else return mid;
            }
            // find nothing
            return -1;
        }
        // Big first
        template<typename T>
        int32_t BinarySearchBF(T* sorted, std::size_t size, T& key, std::function<int32_t(T&,T&)> compare) {
            int32_t low = 0;
            int32_t high = size - 1;
            while(low <= high) {
                int32_t mid = low + (high - low) / 2;
                int32_t result = compare(key, sorted[mid]);
                if(result > 0) high = mid - 1;
                else if(result < 0) low = mid + 1;
                else return mid;
            }
            // find nothing
            return -1;
        }
    }
}