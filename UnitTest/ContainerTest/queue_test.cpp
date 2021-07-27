#include "Memory/MemoryCheck.h"
#include "Containers/Queue/Queue.h"
#include <random>

using namespace Rocket;

int main() {
    Queue<int32_t> queue;
    int count = 1000;

    for(int i = 0; i < count; ++i) {
        queue.Enqueue(std::rand());
    }

    for(int i = 0; i < count; ++i) {
        queue.Dequeue();
        //std::cout << "Current Data: " << i << " , " << stack.Pop() << std::endl;
    }

    std::cout << "Is Empty: " << queue.IsEmpty() << std::endl;

    return 0;
}