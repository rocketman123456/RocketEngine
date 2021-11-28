#include "Memory/LinearAllocator.h"
#include "Log/Log.h"

#include <cstdlib>
#include <cassert>
#include <algorithm>

namespace Rocket {
    void* LinearAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
        std::size_t padding = 0;
        std::size_t paddedAddress = 0;
        const std::size_t currentAddress = (std::size_t)start_ptr + offset;

        if (alignment != 0 && offset % alignment != 0) {
            // Alignment is required. Find the next aligned memory address and update offset
            padding = AllocatorUtils::CalculatePadding(currentAddress, alignment);
        }

        if (offset + padding + size > total_size) {
            return nullptr;
        }

        offset += padding;
        const std::size_t nextAddress = currentAddress + padding;
        offset += size;

        RK_TRACE(Memory, "Linear\t@Curr {}\t@Next {}\tSize {}\tTotal {}\tPad {}", currentAddress, nextAddress, size, offset, padding);

        used = offset;
        peak = std::max(peak, used);

        return (void*) nextAddress;
    }

    void LinearAllocator::Free(void* ptr) {
        assert(false && "Use Reset() method");
    }

    void LinearAllocator::Init() {
        if (start_ptr != nullptr) {
            free(start_ptr);
        }
        start_ptr = malloc(total_size);
        offset = 0;
    }

    void LinearAllocator::Reset() {
        offset = 0;
        used = 0;
        peak = 0;
    }
}
