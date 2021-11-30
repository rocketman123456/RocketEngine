#include "Memory/MiAllocator.h"
#include "Log/Log.h"

#include <mimalloc.h>

namespace Rocket {
    void* MiAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
        void* ptr = mi_malloc(size);
        RK_TRACE(Memory, "Mimalloc\t@Alloc {}\tSize {}", ptr, size);
        return ptr;
    }

    void MiAllocator::Free(void* ptr) {
        RK_TRACE(Memory, "Mimalloc\t@Free {}", ptr);
        mi_free(ptr);
    }
}
