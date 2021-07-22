#define ENABLE_NEW_DELETE_TRACE_DUMP
#include "Memory/MemoryCheck.h"
#include "Containers/Bag/Bag.h"
#include <random>

using namespace Rocket;

int main() {
    int count = 1000;
    Bag<int32_t> bag;
    for(int i = 0; i < count; ++i) {
        bag.Add(std::rand());
    }
    std::cout << "Current Size: " << bag.CurrentSize() << std::endl;

    auto iterator = bag.GetIterator();
    int iter_count = 0;
    while(iterator.HasNext()) {
        int32_t temp = iterator.Next();
        //std::cout << "Iterator Get: " << temp << std::endl;
        iter_count++;
    }
    std::cout << "Iterator Count: " << iter_count << std::endl;

    return 0;
}