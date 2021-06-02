#include <iostream>
#include <memory>

//#include "Memory/AllocatorBasic.h"
//#include "Debug/MemLeak.h"

using namespace std;

int main(int argc, char** argv)
{
    cout << "Hello World!" << endl;

    int* ptr = new int[10];
    delete[] ptr;

    ptr = new int[10];
    ptr = new int[10];
    delete[] ptr;

    //{
    //    Rocket::default_allocator<int> alloc;
    //    auto u_ptr = allocate_unique<int, Rocket::default_allocator<int>>(alloc, 100);
    //    auto s_ptr_1 = allocate_shared<int, Rocket::default_allocator<int>>(alloc, 100);
    //    auto s_ptr_2 = allocate_shared<int, Rocket::default_allocator<int>>(alloc, 200);
    //}

    ptr = new int[10];
    
    return 0;
}
