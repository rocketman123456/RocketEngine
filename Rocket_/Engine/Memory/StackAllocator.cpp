#include "Memory/StackAllocator.h"
#include "Log/Log.h"

namespace Rocket {
    StackAllocator::StackAllocator(std::size_t total) : Allocator(total) {
    }

    StackAllocator::~StackAllocator() {
        if(start_ptr) free(start_ptr);
        start_ptr = nullptr;
    }

    void StackAllocator::Init() {
        if (start_ptr != nullptr) {
            free(start_ptr);
        }
        start_ptr = malloc(total_size);
        offset = 0;
    }

    void StackAllocator::Reset() {
        offset = 0;
        used = 0;
        peak = 0;
    }

    void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
        const std::size_t currentAddress = (std::size_t)start_ptr + offset;

        std::size_t padding = AllocatorUtils::CalculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));

        if (offset + padding + size > total_size) {
            return nullptr;
        }
        offset += padding;

        const std::size_t nextAddress = currentAddress + padding;
        const std::size_t headerAddress = nextAddress - sizeof (AllocationHeader);
        AllocationHeader allocationHeader = { static_cast<char>(padding) };
        AllocationHeader* headerPtr = (AllocationHeader*) headerAddress;
        headerPtr = &allocationHeader;
        
        offset += size;

        RK_TRACE(Memory, "StackAlloc\t@Curr {}\t@Next {}\tOffset {}\tPad {}\tTotal {}", currentAddress, nextAddress, offset, padding, used);

        used = offset;
        peak = std::max(peak, used);

        return (void*) nextAddress;
    }

    void StackAllocator::Free(void* ptr) {
        // Move offset back to clear address
        const std::size_t currentAddress = (std::size_t) ptr;
        const std::size_t headerAddress = currentAddress - sizeof (AllocationHeader);
        const AllocationHeader * allocationHeader{ (AllocationHeader *) headerAddress};

        offset = currentAddress - allocationHeader->padding - (std::size_t)start_ptr;
        used = offset;

        RK_TRACE(Memory, "StackFree\t@Curr {}\t@Free {}\tOffset {}", currentAddress, (std::size_t)start_ptr + offset, offset);
    }
}