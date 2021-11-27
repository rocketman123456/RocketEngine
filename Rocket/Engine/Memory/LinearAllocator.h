#pragma once
#include "Memory/Allocator.h"

namespace Rocket {
    class LinearAllocator : _implements_ Allocator {
    protected:
        void* start_ptr = nullptr;
        std::size_t offset = 0;
    public:
        LinearAllocator(const std::size_t totalSize) : Allocator(totalSize) {}
        virtual ~LinearAllocator() { std::free(start_ptr); }

        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr) override;

        virtual void Init() override;
        virtual void Reset() override;
    private:
        LinearAllocator(LinearAllocator &linearAllocator);
    };
}
