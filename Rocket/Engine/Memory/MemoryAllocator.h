#pragma once
#include "Memory/MemoryDefine.h"

#include <new>
#include <cstddef>
#include <type_traits>
#include <stdexcept>
#include <exception>
#include <limits>
#include <iostream>

namespace Rocket {
    // A Typical C++ Allocator for stl
    // Can also be used in normal memory allocation
    template <class T>
    class MyAllocator {
    public:
        // type definitions
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using void_pointer = void*;
        using const_void_pointer = const void*;
        using size_type = std::size_t;
        using different_type = std::ptrdiff_t;

        // rebind allocator to type U
        template <class U>
        struct rebind {
            typedef MyAllocator<U> other;
        };

        // return address of values
        inline pointer address(reference value) const noexcept { return &value; }
        inline const_pointer address(const_reference value) const noexcept { return &value; }

        // constructors and destructor
        // - nothing to do because the allocator has no state
        MyAllocator() noexcept {}
        MyAllocator(const MyAllocator&) noexcept {}
        template <class U>
        MyAllocator(const MyAllocator<U>&) noexcept {}
        ~MyAllocator() noexcept {}

        // return maximum number of elements that can be allocated
        size_type max_size() const noexcept {
            return std::numeric_limits<std::size_t>::max() / sizeof(T);
        }

        // allocate but don't initialize num elements of type T
        pointer allocate(size_type num, const_void_pointer = static_cast<const_void_pointer>(0)) {
            if (num > this->max_size())
                throw std::bad_alloc();
            // print message and allocate memory with global new
            pointer ret = nullptr;
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                ret = static_cast<pointer>(::operator new(num * sizeof(value_type), std::align_val_t(alignof(value_type))));
            else 
                ret = static_cast<pointer>(::operator new(num * sizeof(value_type)));
            std::cerr << "allocate " << num << " element(s)"
                      << " of size " << sizeof(T)
                      << " allocated at: " << (void *)ret << std::endl;
            return ret;
        }

        // deallocate storage p of deleted elements
        void deallocate(pointer p, size_type num) {
            // print message and deallocate memory with global delete
            std::cerr << "deallocate " << num << " element(s)"
                      << " of size " << sizeof(T)
                      << " at: " << (void *)p << std::endl;
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                ::operator delete(static_cast<void_pointer>(p), num * sizeof(value_type), std::align_val_t(alignof(value_type)));
            else
                ::operator delete(static_cast<void_pointer>(p), num * sizeof(value_type));
        }

        // initialize elements of allocated storage p with value value
        template <class U, typename ... Args>
        void construct(U* p, Args&& ... args) noexcept {
            // initialize memory with placement new
            ::new ((void_pointer)p) U(std::forward<Args>(args)...);
        }

        // destroy elements of initialized storage p
        void destroy(pointer p) noexcept {
            // destroy objects by calling their destructor
            p->~T();
        }
    };

    // return that all specializations of this allocator are interchangeable
    template <class T1, class T2>
    bool operator == (const MyAllocator<T1> &, const MyAllocator<T2> &) noexcept {
        return true;
    }

    template <class T1, class T2>
    bool operator != (const MyAllocator<T1> &, const MyAllocator<T2> &) noexcept {
        return false;
    }
}
