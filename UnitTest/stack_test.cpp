#define ENABLE_NEW_DELETE_TRACE_DUMP
#include "Memory/MemoryCheck.h"
#include "DataStructure/Stack.h"
#include <random>

using namespace Rocket;

int main() {
    Stack<int32_t> stack;
    int count = 1000;

    for(int i = 0; i < count; ++i) {
        stack.Push(std::rand());
    }

    for(int i = 0; i < count; ++i) {
        stack.Pop();
        //std::cout << "Current Data: " << i << " , " << stack.Pop() << std::endl;
    }

    std::cout << "Is Empty: " << stack.IsEmpty() << std::endl;

    return 0;
}