#pragma once
#include "Memory/Allocator.h"

namespace Rocket {
    class CAllocator : public Allocator {
    public:
        CAllocator() : Allocator(0) {};
        virtual ~CAllocator() = default;

        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr) override;

        virtual void Init() override {}
        virtual void Reset() override {}
    };
}
