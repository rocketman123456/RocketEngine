#include "Memory/MemoryAllocator.h"

#include <vector>

using namespace Rocket;

struct Count {
    int i;
};

int main() {
    std::vector<int, MyAllocator<int> > vec;
    std::vector<Count, MyAllocator<Count> > vec_count;

    for(int i = 0; i < 100; ++i) {
        vec.push_back(i);
        vec_count.push_back({i});
    }

    return 0;
}