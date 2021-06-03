#include <iostream>
#include <memory>

//#include "Memory/AllocatorBasic.h"
//#include "Debug/MemLeak.h"

using namespace std;

int main(int argc, char** argv)
{
    cout << "Hello World!" << endl;

    int* ptr = new int[16];
    delete[] ptr;

    int* ptr_1 = new int[8];
    int* ptr_2 = new int[16];
    delete[] ptr_1;

    //{
    //    Rocket::default_allocator<int> alloc;
    //    auto u_ptr = allocate_unique<int, Rocket::default_allocator<int>>(alloc, 100);
    //    auto s_ptr_1 = allocate_shared<int, Rocket::default_allocator<int>>(alloc, 100);
    //    auto s_ptr_2 = allocate_shared<int, Rocket::default_allocator<int>>(alloc, 200);
    //}

    int* ptr_3 = new int[16];
    
    return 0;
}
