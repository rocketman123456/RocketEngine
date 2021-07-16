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
    
    List<double> list_copy(list);
    List<double> list_copy_2;
    list_copy_2 = list;

    for(int i = 0; i < 199; ++i)
        list.RemoveFront();
    //for(int i = 0; i < 100; ++i)
    //    list.RemoveBack();

    return 0;
}