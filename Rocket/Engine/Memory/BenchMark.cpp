#include "Memory/BenchMark.h"
#include "Utils/IO.h"

#include <iostream>
#include <stdlib.h>     // srand, rand
#include <cassert>

#define OPERATIONS (10)

namespace Rocket {
    void Benchmark::SingleAllocation(Allocator* allocator, const std::size_t size, const std::size_t alignment) {
        std::cout << "BENCHMARK: ALLOCATION" << IO::endl;
        std::cout << "\tSize:     \t" << size << IO::endl;
        std::cout << "\tAlignment\t" << alignment << IO::endl;

        StartRound();

        allocator->Init();

        auto operations = 0u;

        while (operations < operation_count) {
            allocator->Allocate(size, alignment);
            ++operations;
        }
        
        FinishRound();

        BenchmarkResults results = BuildResults(operation_count, std::move(time_elapsed), allocator->peak);

        PrintResults(results);
    }

    void Benchmark::SingleFree(Allocator* allocator, const std::size_t size, const std::size_t alignment) {
        std::cout << "BENCHMARK: ALLOCATION/FREE" << IO::endl;
        std::cout << "\tSize:     \t" << size << IO::endl;
        std::cout << "\tAlignment\t" << alignment << IO::endl;

        // BUG: (https://github.com/mtrebi/memory-allocators/issues/6)
        // void* addresses[m_nOperations];
        void* addresses[OPERATIONS];

        StartRound();

        allocator->Init();

        auto operations = 0u;

        while (operations < operation_count) {
            addresses[operations] = allocator->Allocate(size, alignment);
            ++operations;
        }

        while (operations) {
            allocator->Free(addresses[--operations]);
        }

        FinishRound();

        BenchmarkResults results = BuildResults(operation_count, std::move(time_elapsed), allocator->peak);

        PrintResults(results);
    }

    void Benchmark::MultipleAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments) {
        assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");

        for (auto i = 0u; i < allocationSizes.size(); ++i) {
            SingleAllocation(allocator, allocationSizes[i], alignments[i]);
        }
    }

    void Benchmark::MultipleFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments) {
        assert(allocationSizes.size() == alignments.size() && "Allocation sizes and Alignments must have same length");

        for (auto i = 0u; i < allocationSizes.size(); ++i) {
            SingleFree(allocator, allocationSizes[i], alignments[i]);
        }
    }

    void Benchmark::RandomAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments) {
        
        // NOTE: Is this actually initializing the RNG? Jose Fernando Lopez Fernandez 11/07/2018 @ 12:54am (UTC)
        srand(1);

        std::cout << "\tBENCHMARK: ALLOCATION" << IO::endl;

        StartRound();

        std::size_t allocation_size;
        std::size_t alignment;

        allocator->Init();

        auto operations = 0u;

        while (operations < operation_count) {
            this->RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
            allocator->Allocate(allocation_size, alignment);
            ++operations;
        }
        
        FinishRound();

        BenchmarkResults results = BuildResults(operation_count, std::move(time_elapsed), allocator->peak);
        
        PrintResults(results);
    }

    void Benchmark::RandomFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments) {
        
        // NOTE: Is this actually initializing the RNG? Jose Fernando Lopez Fernandez 11/07/2018 @ 1:51am (UTC)
        srand(1);

        std::cout << "\tBENCHMARK: ALLOCATION/FREE" << IO::endl;

        StartRound();

        void* addresses[OPERATIONS];

        std::size_t allocation_size;
        std::size_t alignment;

        allocator->Init();

        auto operations = 0u;

        while (operations < operation_count) {
            this->RandomAllocationAttr(allocationSizes, alignments, allocation_size, alignment);
            addresses[operations] = allocator->Allocate(allocation_size, alignment);
            ++operations;
        }

        while (operations) {
            allocator->Free(addresses[--operations]);
        }

        FinishRound();

        BenchmarkResults results = BuildResults(operation_count, std::move(time_elapsed), allocator->peak);

        PrintResults(results);

    }

    void Benchmark::PrintResults(const BenchmarkResults& results) const {
        std::cout << "\tRESULTS:" << IO::endl;
        std::cout << "\t\tOperations:    \t" << results.operations << IO::endl;
        std::cout << "\t\tTime elapsed: \t" << results.milliseconds.count() << " ms" << IO::endl;
        std::cout << "\t\tOp per sec:    \t" << results.operations_per_sec << " ops/ms" << IO::endl;
        std::cout << "\t\tTimer per op:  \t" << results.time_per_operation << " ms/ops" << IO::endl;
        std::cout << "\t\tMemory peak:   \t" << results.memory_peak << " bytes" << IO::endl;

        std::cout << IO::endl;
    }

    const BenchmarkResults Benchmark::BuildResults(std::size_t nOperations, std::chrono::milliseconds&& elapsedTime, const std::size_t memoryPeak) const {
        BenchmarkResults results;

        results.operations = nOperations;
        results.milliseconds = std::move(elapsedTime);
        results.operations_per_sec = results.operations / static_cast<double>(results.milliseconds.count());
        results.time_per_operation = static_cast<double>(results.milliseconds.count()) / static_cast<double>(results.operations);
        results.memory_peak = memoryPeak;

        return results;
    }

    void Benchmark::RandomAllocationAttr(const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments, std::size_t & size, std::size_t & alignment) {
        const int r = rand() % allocationSizes.size();
        size = allocationSizes[r];
        alignment = alignments[r];
    }
}
