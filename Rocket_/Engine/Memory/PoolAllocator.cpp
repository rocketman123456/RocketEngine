#include "Memory/PoolAllocator.h"
#include "Log/Log.h"

#include <cassert>
#include <algorithm>

namespace Rocket {
    PoolAllocator::PoolAllocator(std::size_t total, const std::size_t chunk) : Allocator(total) {
        assert(chunk >= 8 && "Chunk size must be greater or equal to 8");
        assert(total % chunk == 0 && "Total Size must be a multiple of Chunk Size");
        this->chunk_size = chunk;
    }

    PoolAllocator::~PoolAllocator() {
        free(start_ptr);
    }

    void PoolAllocator::Init() {
        start_ptr = malloc(total_size);
        this->Reset();
    }

    void PoolAllocator::Reset() {
        used = 0;
        peak = 0;
        // Create a linked-list with all free positions
        const int chunks = total_size / chunk_size;
        for (int i = 0; i < chunks; ++i) {
            std::size_t address = (std::size_t) start_ptr + i * chunk_size;
            free_list.push((Node *) address);
        }
    }

    void* PoolAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
        assert(size == this->chunk_size && "Allocation size must be equal to chunk size");

        Node * free_position = free_list.pop();

        assert(free_position != nullptr && "The pool allocator is full");

        used += chunk_size;
        peak = std::max(peak, used);

        RK_TRACE(Memory, "PoolAlloc\t@Start {}\t@Curr {}\tTotal {}", start_ptr, (void*)free_position, used);

        return (void*) free_position;
    }

    void PoolAllocator::Free(void* ptr) {
        used -= chunk_size;

        free_list.push((Node*) ptr);

        RK_TRACE(Memory, "PoolFree\t@Start {}\t@Curr {}\tTotal {}", start_ptr, ptr, used);
    }
}