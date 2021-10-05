#pragma once

// TODO : add define for Memory Alignment
// For Classes Needed Memory Alignment

#define ALIGNED_OPERATOR_NEW \
    void* operator new(std::size_t count) {\
        void* original = ::operator new(count + 32);\
        void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~size_t(32 - 1)) + 32);\
        *(reinterpret_cast<void**>(aligned) - 1) = original;\
        return aligned;\
    }\
    void operator delete(void* ptr) {\
        ::operator delete(*(reinterpret_cast<void**>(ptr) - 1));\
    }
