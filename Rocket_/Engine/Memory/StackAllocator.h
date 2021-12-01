#pragma once
#include "Memory/Allocator.h"

namespace Rocket {
    class StackAllocator : public Allocator {
    public:
        StackAllocator(std::size_t total);
        StackAllocator(const StackAllocator& allocator) = delete;
        StackAllocator(StackAllocator&& allocator) = delete;
        virtual ~StackAllocator();

        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr) override;

        virtual void Init() override;
        virtual void Reset() override;

    private:
        struct AllocationHeader {
            char padding;
        };
        void* start_ptr = nullptr;
        std::size_t offset = 0;
    };
}
