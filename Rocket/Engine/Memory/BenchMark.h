#pragma once
#include "Memory/Allocator.h"

#include <time.h>   // timespec
#include <cstddef>  // std::size_t
#include <chrono>
#include <ratio>
#include <vector>

#define OPERATIONS (10)

namespace Rocket {
    struct BenchmarkResults {
        std::size_t operations;
        std::chrono::milliseconds milliseconds;
        double operations_per_sec;
        double time_per_operation;
        std::size_t memory_peak;
    };

    class Benchmark {
    public:
        Benchmark() = delete;
        explicit Benchmark(const unsigned int operations) : operation_count { operations } { }

        void SingleAllocation(Allocator* allocator, const std::size_t size, const std::size_t alignment);
        void SingleFree(Allocator* allocator, const std::size_t size, const std::size_t alignment);

        void MultipleAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);
        void MultipleFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);

        void RandomAllocation(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);
        void RandomFree(Allocator* allocator, const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments);

    private:
        void PrintResults(const BenchmarkResults& results) const;

        void RandomAllocationAttr(const std::vector<std::size_t>& allocationSizes, const std::vector<std::size_t>& alignments, std::size_t & size, std::size_t & alignment);

        const BenchmarkResults BuildResults(std::size_t nOperations, std::chrono::milliseconds&& ellapsedTime, const std::size_t memoryUsed) const;
        
        inline void SetStartTime() noexcept { start = std::chrono::high_resolution_clock::now(); }
        inline void SetFinishTime() noexcept { finish = std::chrono::high_resolution_clock::now(); }
        inline void SetElapsedTime() noexcept { time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start); }
        
        void StartRound() noexcept { SetStartTime(); }
        void FinishRound() noexcept {
            SetFinishTime();
            SetElapsedTime();
        }

    private:
        std::size_t operation_count;

        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::time_point<std::chrono::high_resolution_clock> finish;

        std::chrono::milliseconds time_elapsed;
    };
}
