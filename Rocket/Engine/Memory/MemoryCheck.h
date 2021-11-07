// https://vorbrodt.blog/2021/05/27/how-to-detect-memory-leaks/
#pragma once

// #ifdef new
// #undef new
// #endif

#ifdef RK_MEMORY_CHECK

#include <new>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <unordered_set>
#include <iostream>
#include <ostream>
#include <cstdlib>

namespace Rocket::Memory::detail {
	using string_t = const char*;
	struct new_entry_t;
}

void* operator new (std::size_t n);
void* operator new [] (std::size_t n);
void operator delete (void* ptr) noexcept;
void operator delete [] (void* ptr) noexcept;

void* operator new (std::size_t n, Rocket::Memory::detail::new_entry_t&& entry);

void* operator new (std::size_t n, Rocket::Memory::detail::string_t file, int line, Rocket::Memory::detail::string_t func);
void* operator new [] (std::size_t n, Rocket::Memory::detail::string_t file, int line, Rocket::Memory::detail::string_t func);
void operator delete (void* ptr, Rocket::Memory::detail::string_t, int, Rocket::Memory::detail::string_t) noexcept;
void operator delete [] (void* ptr, Rocket::Memory::detail::string_t, int, Rocket::Memory::detail::string_t) noexcept;

// #ifndef new
// #define new new(__FILE__, __LINE__, __proc__)
// #endif
#endif
