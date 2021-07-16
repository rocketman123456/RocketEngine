#include "Algorism/Search.h"
#include <iostream>

int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main() {
    int key = 8;

    auto result = Rocket::Algorism::BinarySearch<int>(array, sizeof(array)/sizeof(array[0]), key,
        [](int k1, int k2) -> int32_t { return k1-k2; }
    );

    std::cout << "Binary Search Result: " << result << std::endl;

    return 0;
}