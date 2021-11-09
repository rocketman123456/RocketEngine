#include "Memory/MemoryCheck.h"
#include "Containers/Stack/Stack.h"
#include "Containers/Stack/FixedStack.h"
#include "Containers/Stack/ListStack.h"

#include <random>
#include <iostream>

using namespace Rocket;

int main() {
    Stack<int32_t> stack;
    int count = 5;

    for(int i = 0; i < count; ++i) {
        stack.push(std::rand());
    }

    for(int i = 0; i < count; ++i) {
        stack.pop();
    }

    std::cout << "Is Empty: " << stack.empty() << std::endl;

    const int32_t count_2 = 100;
    FixedStack<int32_t, count_2> stack_f;
    ListStack<int32_t> stack_l;

    for(int i = 0; i < count_2; ++i) {
        stack_f.push(std::rand());
        stack_l.push(std::rand());
    }

    return 0;
}
