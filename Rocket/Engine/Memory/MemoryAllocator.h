#pragma once

#include <cstddef>
#include <new>
#include <type_traits>
#include <stdexcept>
#include <exception>
#include <limits>

namespace Rocket
{
    // TODO : implement different allocator
    // template <typename T>
    // class Allocator {
    // public:
    //     using value_type = T;
    //     using pointer = T*;
    //     using const_pointer = const T*;
    //     using void_pointer = void*;
    //     using const_void_pointer = const void*;
    //     using size_type = std::size_t;
    //     using different_type = std::ptrdiff_t;

    //     Allocator() noexcept = default;
    //     Allocator(const Allocator &) noexcept = default;
    //     ~Allocator() = default;

    //     template <class U>
    //     Allocator(const Allocator<U> &) noexcept {}

    //     [[nodiscard]] T* allocate(std::size_t n) {
    //         if (n > std::size_t(-1) / sizeof(T)) {
    //             throw std::bad_array_new_length();
    //         }
            
    //         if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
    //             return static_cast<T *>( ::operator new(n * sizeof(T), static_cast<std::align_val_t>(alignof(T))) );
    //         else
    //             return static_cast<T *>( ::operator new(n * sizeof(T)) );
    //     }

    //     void deallocate(T *p, std::size_t n) {
    //         if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
    //             return ::operator delete(p, n * sizeof(T), static_cast<std::align_val_t>(alignof(T)));
    //         else
    //             return ::operator delete(p, n * sizeof(T));
    //     }
    // };

    // template <class T, class U>
    // bool operator == (const Allocator<T> &, const Allocator<U> &) noexcept {
    //     return true;
    // }

    // template <class T, class U>
    // bool operator != (const Allocator<T> &, const Allocator<U> &) noexcept {
    //     return false;
    // }

    template <class T>
    class MyAllocator {
    public:
        // type definitions
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using void_pointer = void*;
        using const_void_pointer = const void*;
        using size_type = std::size_t;
        using different_type = std::ptrdiff_t;

        // rebind allocator to type U
        template <class U>
        struct rebind {
            typedef MyAlloc<U> other;
        };

        // return address of values
        pointer address(reference value) const {
            return &value;
        }

        const_pointer address(const_reference value) const {
            return &value;
        }

        // constructors and destructor
        // - nothing to do because the allocator has no state
        MyAllocator() throw() {}
        MyAllocator(const MyAllocator &) throw() {}
        template <class U>
        MyAllocator(const MyAllocator<U> &) throw() {}
        ~MyAllocator() throw() {}

        // return maximum number of elements that can be allocated
        size_type max_size() const throw() {
            return std::numeric_limits<std::size_t>::max() / sizeof(T);
        }

        // allocate but don't initialize num elements of type T
        pointer allocate(size_type num, const void* = 0) {
            // print message and allocate memory with global new
            std::cerr << "allocate " << num << " element(s)"
                      << " of size " << sizeof(T) << std::endl;
            pointer ret = (pointer)(::operator new(num * sizeof(T)));
            std::cerr << " allocated at: " << (void *)ret << std::endl;
            return ret;
        }

        // initialize elements of allocated storage p with value value
        void construct(pointer p, const T &value) {
            // initialize memory with placement new
            new ((void *)p) T(value);
        }

        // destroy elements of initialized storage p
        void destroy(pointer p) {
            // destroy objects by calling their destructor
            p->~T();
        }

        // deallocate storage p of deleted elements
        void deallocate(pointer p, size_type num) {
            // print message and deallocate memory with global delete
            std::cerr << "deallocate " << num << " element(s)"
                      << " of size " << sizeof(T)
                      << " at: " << (void *)p << std::endl;
            ::operator delete((void *)p);
        }
    };

    // return that all specializations of this allocator are interchangeable
    template <class T1, class T2>
    bool operator == (const MyAllocator<T1> &, const MyAllocator<T2> &) throw() {
        return true;
    }
    template <class T1, class T2>
    bool operator != (const MyAllocator<T1> &, const MyAllocator<T2> &) throw() {
        return false;
    }
}
