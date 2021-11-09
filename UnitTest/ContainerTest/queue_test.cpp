#include "Memory/MemoryCheck.h"
#include "Containers/Queue/Queue.h"

#include <random>
#include <iostream>

using namespace Rocket;

int main() {
    Queue<int32_t> queue;
    int count = 1000;

    for(int i = 0; i < count; ++i) {
        queue.enqueue(std::rand());
    }

    for(int i = 0; i < count; ++i) {
        queue.dequeue();
    }

    std::cout << "Is Empty: " << queue.empty() << std::endl;

    return 0;
}
