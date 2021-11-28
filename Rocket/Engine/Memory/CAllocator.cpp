#include "Memory/CAllocator.h"
#include "Log/Log.h"

#include <mimalloc.h>

namespace Rocket {
    void* CAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
        void* ptr = std::malloc(size);//mi_malloc(size);
        RK_TRACE(Memory, "Normal\t@Alloc {}\tSize {}", ptr, size);
        return ptr;
    }

    void CAllocator::Free(void* ptr) {
        RK_TRACE(Memory, "Normal\t@Free {}", ptr);
        std::free(ptr);//mi_free(ptr);
    }
}
