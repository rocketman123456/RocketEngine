#define ENABLE_NEW_DELETE_TRACE_DUMP
#include "Memory/MemoryCheck.h"
#include "Containers/List.h"
#include <random>

using namespace Rocket;

int main() {
    List<double> list;

    for(int i = 0; i < 100; ++i)
        list.InsertFront(std::rand());
    for(int i = 0; i < 100; ++i)
        list.InsertBack(std::rand());

    return 0;
}