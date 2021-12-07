#pragma once
#include "Core/Declare.h"

#include <memory>
#define CLASS_PTR(_class_) \
    using _class_##Ptr = std::shared_ptr<_class_>;\
    using _class_##Weak = std::weak_ptr<_class_>;

// ----------------------------------------------------------------------------
// This header provides convenient overrides for the new and
// delete operations in C++.
//
// This header should be included in only one source file!
//
// On Windows, or when linking dynamically with mimalloc, these
// can be more performant than the standard new-delete operations.
// See <https://en.cppreference.com/w/cpp/memory/new/operator_new>
// ---------------------------------------------------------------------------

#if defined(__cplusplus)
// #include <new>
// #include <mimalloc.h>

//     void operator delete(void* p) noexcept              { mi_free(p); };
//     void operator delete[](void* p) noexcept            { mi_free(p); };

//     void* operator new(std::size_t n) noexcept(false)   { return mi_new(n); }
//     void* operator new[](std::size_t n) noexcept(false) { return mi_new(n); }

//     void* operator new  (std::size_t n, const std::nothrow_t& tag) noexcept { (void)(tag); return mi_new_nothrow(n); }
//     void* operator new[](std::size_t n, const std::nothrow_t& tag) noexcept { (void)(tag); return mi_new_nothrow(n); }

// #if (__cplusplus >= 201402L || _MSC_VER >= 1916)
//     void operator delete  (void* p, std::size_t n) noexcept { mi_free_size(p,n); };
//     void operator delete[](void* p, std::size_t n) noexcept { mi_free_size(p,n); };
// #endif

// #if (__cplusplus > 201402L || defined(__cpp_aligned_new))
//     void operator delete  (void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
//     void operator delete[](void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
//     void operator delete  (void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };
//     void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };

//     void* operator new( std::size_t n, std::align_val_t al)   noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
//     void* operator new[]( std::size_t n, std::align_val_t al) noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
//     void* operator new  (std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
//     void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
// #endif
#endif

// #define ALIGNED_OPERATOR_NEW \
//     void* operator new(std::size_t count) {\
//         void* original = ::operator new(count + 32);\
//         void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~size_t(32 - 1)) + 32);\
//         *(reinterpret_cast<void**>(aligned) - 1) = original;\
//         return aligned;\
//     }\
//     void *operator new[](std::size_t count) { \
//         void* original = ::operator new(count + 32);\
//         void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~size_t(32 - 1)) + 32);\
//         *(reinterpret_cast<void**>(aligned) - 1) = original;\
//         return aligned;\
//     } \
//     void operator delete(void* ptr) {\
//         ::operator delete(*(reinterpret_cast<void**>(ptr) - 1));\
//     }\
//     void operator delete[](void * ptr) {\
//         ::operator delete(*(reinterpret_cast<void**>(ptr) - 1));\
//     }
