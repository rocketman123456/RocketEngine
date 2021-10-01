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
    template <typename T>
    class allocator {
    public:
        using value_type = T;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        allocator() noexcept = default;
        allocator(const allocator &) noexcept = default;
        ~allocator() = default;

        template <class U>
        allocator(const allocator<U> &) noexcept {}

        [[nodiscard]] T *allocate(std::size_t n) {
            if (n > std::size_t(-1) / sizeof(T)) {
                throw std::bad_array_new_length();
            }
            
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                return static_cast<T *>( ::operator new(n * sizeof(T), static_cast<std::align_val_t>(alignof(T))) );
            else
                return static_cast<T *>( ::operator new(n * sizeof(T)) );
        }

        void deallocate(T *p, std::size_t n) {
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                return ::operator delete(p, n * sizeof(T),
                                         static_cast<std::align_val_t>(alignof(T)));
            else
                return ::operator delete(p, n * sizeof(T));
        }
    };

    template <class T, class U>
    bool operator==(const allocator<T> &, const allocator<U> &) noexcept {
        return true;
    }

    template <class T, class U>
    bool operator!=(const allocator<T> &, const allocator<U> &) noexcept {
        return false;
    }

    template <class T>
    class MyAlloc
    {
    public:
        // type definitions
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        // rebind allocator to type U
        template <class U>
        struct rebind
        {
            typedef MyAlloc<U> other;
        };

        // return address of values
        pointer address(reference value) const
        {
            return &value;
        }

        const_pointer address(const_reference value) const
        {
            return &value;
        }

        /* constructors and destructor
        * - nothing to do because the allocator has no state
        */
        MyAlloc() throw()
        {
        }
        MyAlloc(const MyAlloc &) throw()
        {
        }
        template <class U>
        MyAlloc(const MyAlloc<U> &) throw()
        {
        }
        ~MyAlloc() throw()
        {
        }

        // return maximum number of elements that can be allocated
        size_type max_size() const throw()
        {
            return std::numeric_limits<std::size_t>::max() / sizeof(T);
        }

        // allocate but don't initialize num elements of type T
        pointer allocate(size_type num, const void * = 0)
        {
            // print message and allocate memory with global new
            std::cerr << "allocate " << num << " element(s)"
                      << " of size " << sizeof(T) << std::endl;
            pointer ret = (pointer)(::operator new(num * sizeof(T)));
            std::cerr << " allocated at: " << (void *)ret << std::endl;
            return ret;
        }

        // initialize elements of allocated storage p with value value
        void construct(pointer p, const T &value)
        {
            // initialize memory with placement new
            new ((void *)p) T(value);
        }

        // destroy elements of initialized storage p
        void destroy(pointer p)
        {
            // destroy objects by calling their destructor
            p->~T();
        }

        // deallocate storage p of deleted elements
        void deallocate(pointer p, size_type num)
        {
            // print message and deallocate memory with global delete
            std::cerr << "deallocate " << num << " element(s)"
                      << " of size " << sizeof(T)
                      << " at: " << (void *)p << std::endl;
            ::operator delete((void *)p);
        }
    };

    // return that all specializations of this allocator are interchangeable
    template <class T1, class T2>
    bool operator==(const MyAlloc<T1> &, const MyAlloc<T2> &) throw()
    {
        return true;
    }
    template <class T1, class T2>
    bool operator!=(const MyAlloc<T1> &, const MyAlloc<T2> &) throw()
    {
        return false;
    }
}