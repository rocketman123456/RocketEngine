#include "Memory/StackAllocator.h"
#include "Log/Log.h"

namespace Rocket {
    StackAllocator::StackAllocator(std::size_t total) : Allocator(total) {
    }

    StackAllocator::~StackAllocator() {
    }

    void StackAllocator::Init() {
    }

    void StackAllocator::Reset() {
    }

    void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
    }

    void StackAllocator::Free(void* ptr) {
    }
}