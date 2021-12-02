#include "Container/LRUCache.h"

using namespace Rocket;

#include <iostream>

int main() {
    LRUCache<int, double> lru(5);

    for(int i = 0; i < 10; ++i) {
        lru.put(i, i);
    }

    for(int i = 0; i < 10; ++i) {
        std::cout << lru.get(i) << std::endl;
    }

    return 0;
}