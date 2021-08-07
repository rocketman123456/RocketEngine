#include "Memory/MemoryCheck.h"

#ifdef RK_MEMORY_CHECK
#include <thread>
#include <mutex>

#ifdef new
#undef new
#endif

static inline std::mutex memory_allocate_mutex_s;

// create global dump variable, when program exit, it will use ~__dump_all__() automatically and dump all info
namespace { inline const struct __dump_all__ { ~__dump_all__() { Rocket::Memory::dump_all(); } } __dump_all_on_exit__; }


void* operator new (std::size_t n) {
	void* ptr = std::malloc(n);
	if(!ptr) throw std::bad_alloc();
	return ptr;
}

void* operator new (std::size_t n, Rocket::Memory::detail::new_entry_t&& entry) {
	void* ptr = ::operator new(n);
	entry.ptr = ptr;
	try {
		std::unique_lock guard(memory_allocate_mutex_s);
		Rocket::Memory::detail::get_new_entry_set()->insert(std::forward<decltype(entry)>(entry)); 
	}
	catch(...) {}
	return ptr;
}

void* operator new (
	std::size_t n, 
	Rocket::Memory::detail::string_t file, 
	int line, 
	Rocket::Memory::detail::string_t func) {
	return ::operator new(n, Rocket::Memory::detail::new_entry_t{ nullptr, false, n, file, line, func });
}

void* operator new [] (
	std::size_t n, 
	Rocket::Memory::detail::string_t file, 
	int line, 
	Rocket::Memory::detail::string_t func) {
	return ::operator new(n, Rocket::Memory::detail::new_entry_t{ nullptr, true, n, file, line, func });
}

void operator delete (void* ptr) noexcept {
	Rocket::Memory::detail::operator_delete(ptr, false);
}

void operator delete [] (void* ptr) noexcept {
	Rocket::Memory::detail::operator_delete(ptr, true);
}

void operator delete (
	void* ptr, 
	Rocket::Memory::detail::string_t, 
	int, 
	Rocket::Memory::detail::string_t) noexcept {
	Rocket::Memory::detail::operator_delete(ptr, false);
}

void operator delete [] (
	void* ptr, 
	Rocket::Memory::detail::string_t, 
	int, 
	Rocket::Memory::detail::string_t) noexcept {
	Rocket::Memory::detail::operator_delete(ptr, true);
}

#ifndef new
#define new new(__FILE__, __LINE__, __FUNCTION__)
#endif
#endif