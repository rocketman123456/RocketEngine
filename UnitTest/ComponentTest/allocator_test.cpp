#include <iostream>
#include <cstddef>
#include <vector>

#include "Memory/Benchmark.h"
#include "Memory/Allocator.h"
// #include "Memory/StackAllocator.h"
#include "Memory/CAllocator.h"
#include "Memory/LinearAllocator.h"

using namespace Rocket;

int main() {
    const std::size_t A = static_cast<std::size_t>(1e9);
    const std::size_t B = static_cast<std::size_t>(1e8);

    const std::vector<std::size_t> ALLOCATION_SIZES {32, 64, 256, 512, 1024, 2048, 4096};
    const std::vector<std::size_t> ALIGNMENTS {8, 8, 8, 8, 8, 8, 8};

    Allocator * cAllocator = new CAllocator();
    Allocator * linearAllocator = new LinearAllocator(A);
    //linearAllocator->Init();

    Benchmark benchmark(OPERATIONS);

    std::cout << "C" << std::endl;
    benchmark.MultipleAllocation(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.MultipleFree(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.RandomAllocation(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.RandomFree(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);

    std::cout << "LINEAR" << std::endl;
    benchmark.MultipleAllocation(linearAllocator, ALLOCATION_SIZES, ALIGNMENTS);
    benchmark.RandomAllocation(linearAllocator, ALLOCATION_SIZES, ALIGNMENTS); 

    return 0;
}
