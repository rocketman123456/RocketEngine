#pragma once
//#define NEW_OVERLOAD_IMPLEMENTATION_
#include "Debug/MemLeak.h"

#include <new>
#include <memory>
#include <cstddef>
#include <type_traits>

namespace Rocket {
    template <typename T>
    class allocator {
    public:
        using value_type = T;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;

        allocator() noexcept = default;
        allocator(const allocator&) noexcept = default;
        ~allocator() = default;

        template <class U>
        allocator(const allocator<U>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n)
        {
            // check array size
            if (n > std::size_t(-1) / sizeof(T))
                throw std::bad_array_new_length();
            
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                return static_cast<T*>(
                    ::operator new(n * sizeof(T), static_cast<std::align_val_t>(alignof(T)))
                );
            else
                return static_cast<T*>(
                    ::operator new(n * sizeof(T))
                );
        }

        void deallocate(T* p, std::size_t n)
        {
            // check array size
            if (n > std::size_t(-1) / sizeof(T))
                throw std::bad_array_new_length();
            
            if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                return ::operator delete(p, n * sizeof(T), static_cast<std::align_val_t>(alignof(T)));
            else
                return ::operator delete(p, n * sizeof(T));      
        }
    };

    template <class T, class U>
    bool operator==(const allocator<T>&, const allocator<U>&) noexcept
    {
        return true;
    }

    template <class T, class U>
    bool operator!=(const allocator<T>&, const allocator<U>&) noexcept
    {
        return false;
    }
}

template <class T, class A, class ...Args>
auto allocator_new(A& alloc, Args&&... args) {
	using TTraits = typename std::allocator_traits<A>::template rebind_traits<T>;
	using TAlloc = typename std::allocator_traits<A>::template rebind_alloc<T>;

	auto a = TAlloc(alloc);
	auto p = TTraits::allocate(a, 1);

	try {
		TTraits::construct(a, std::__to_address(p), std::forward<Args>(args)...);
		return p;
	} catch(...) {
		TTraits::deallocate(a, p, 1);
		throw;
	}
}

template <class A, class P>
void allocator_delete(A& alloc, P p) {
	using Elem = typename std::pointer_traits<P>::element_type;
	using Traits = typename std::allocator_traits<A>::template rebind_traits<Elem>;

	Traits::destroy(alloc, std::__to_address(p));
	Traits::deallocate(alloc, p, 1);
}

template <class A>
struct allocation_deleter {
	using pointer = typename std::allocator_traits<A>::pointer;

	A a_;  // exposition only

	allocation_deleter(const A& a) noexcept : a_(a) {}

	void operator()(pointer p) {
		allocator_delete(a_, p);
	}
};

// allocate_unique and allocate_shared fix Smart Pointer Bug in operator new and delete

template <class T, class A, class ...Args>
inline auto allocate_unique(A& alloc, Args&&... args) {
	using TAlloc = typename std::allocator_traits<A>::template rebind_alloc<T>;
	return std::unique_ptr<T, allocation_deleter<TAlloc>>(allocator_new<T>(alloc, std::forward<Args>(args)...), alloc);
}

// no need to use custom allocate_shared
//template <class T, class A, class ...Args>
//inline auto allocate_shared(A& alloc, Args&&... args) {
//	//using TAlloc = typename std::allocator_traits<A>::template rebind_alloc<T>;
//    return std::shared_ptr<T>(allocator_new<T>(alloc, std::forward<Args>(args)...));
//}
