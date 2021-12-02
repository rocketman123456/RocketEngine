#include "Algorithm/Search/BinarySearch.h"
#include <iostream>

int64_t array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main() {
    int64_t key = 8;

    {
        auto result = Rocket::Algorism::BinarySearchSF<int64_t>(array, sizeof(array)/sizeof(array[0]), key,
            [](const int64_t& k1, const int64_t& k2) -> int64_t { return (int64_t)(k1 - k2); }
        );
        std::cout << "Binary Search Result: " << result << std::endl;
    }
    

    return 0;
}