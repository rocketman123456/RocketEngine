#define ENABLE_NEW_DELETE_TRACE_DUMP
#include "Memory/MemoryCheck.h"
#include "Containers/Bag/Bag.h"
#include <random>

using namespace Rocket;

int main() {
    Bag<int32_t> bag;
    int count = 1000;

    for(int i = 0; i < count; ++i) {
        bag.Add(std::rand());
    }

    std::cout << "Current Size: " << bag.CurrentSize() << std::endl;

    return 0;
}