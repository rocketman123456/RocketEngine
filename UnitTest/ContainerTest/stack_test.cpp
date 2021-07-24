#define ENABLE_NEW_DELETE_TRACE_DUMP
#include "Memory/MemoryCheck.h"
#include "Containers/Stack/Stack.h"
#include "Containers/Stack/FixStack.h"
#include "Containers/Stack/ListStack.h"
#include <random>

using namespace Rocket;

int main() {
    Stack<int32_t> stack;
    int count = 5;

    for(int i = 0; i < count; ++i) {
        stack.Push(std::rand());
    }

    for(int i = 0; i < count; ++i) {
        stack.Pop();
        //std::cout << "Current Data: " << i << " , " << stack.Pop() << std::endl;
    }

    std::cout << "Is Empty: " << stack.IsEmpty() << std::endl;

    FixStack<int32_t> stack_f(150);
    ListStack<int32_t> stack_l;
    int32_t count_2 = 100;

    for(int i = 0; i < count_2; ++i) {
        stack_f.Push(std::rand());
        stack_l.Push(std::rand());
    }

    return 0;
}