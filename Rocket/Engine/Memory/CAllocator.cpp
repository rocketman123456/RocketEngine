#include "Memory/CAllocator.h"
#include <mimalloc.h>

namespace Rocket {
    void* CAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
        return std::malloc(size);
    }

    void CAllocator::Free(void* ptr) {
        std::free(ptr);
    }
}
