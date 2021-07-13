// https://vorbrodt.blog/2021/05/27/how-to-detect-memory-leaks/

#pragma once

#ifdef new
#undef new
#endif

#include <new>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <unordered_set>
#include <iostream>
#include <ostream>
#include <cstdlib>

namespace Rocket::Memory::detail
{
	template<typename T>
	struct malloc_allocator_t : std::allocator<T>
	{
		malloc_allocator_t() = default;

		template<class U>
		malloc_allocator_t(const malloc_allocator_t<U>&) noexcept {}

		T* allocate(std::size_t n)
		{
			T* ptr = (T*)std::malloc(n * sizeof(T));
			if(!ptr) throw std::bad_alloc();
			return ptr;
		}

		void deallocate(T* ptr, std::size_t) { std::free(ptr); }

		template<typename U>
		struct rebind { typedef malloc_allocator_t<U> other; };
	};

	using string_t = const char*;

	struct new_entry_t
	{
		new_entry_t(void* p = nullptr, bool a = false, std::size_t b = 0,
			string_t f = "N/A", int l = -1, string_t fn = "N/A")
		: ptr{ p }, is_array{ a }, bytes{ b }, file{ f }, line{ l }, func{ fn } {}

		void*       ptr;
		bool        is_array;
		std::size_t bytes;
		string_t    file;
		int         line;
		string_t    func;
	};

	inline std::ostream& operator << (std::ostream& os, const new_entry_t& entry)
	{
		os << entry.bytes << "B leaked using '" << (entry.is_array ? "new[]" : "new")
			<< "' -> '" << entry.file << ":" << entry.line << "' in '" << entry.func << "'";
		return os;
	}

	inline bool operator == (const new_entry_t& lhs, const new_entry_t& rhs) { return lhs.ptr == rhs.ptr; }

	struct new_entry_hash_t : std::hash<void*>
	{
		using base = std::hash<void*>;
		std::size_t operator()(const new_entry_t& entry) const { return base::operator()(entry.ptr); }
	};

	using new_entry_set_t = std::unordered_set<new_entry_t, new_entry_hash_t, std::equal_to<new_entry_t>, malloc_allocator_t<new_entry_t>>;
	using new_entry_list_t = std::vector<new_entry_t, malloc_allocator_t<new_entry_t>>;

	inline auto get_new_entry_set()
	{
		static new_entry_set_t* new_entry_set = []()
		{
			void* raw = std::malloc(sizeof(new_entry_set_t));
			if(!raw) throw std::bad_alloc();
			return new (raw) new_entry_set_t;
		}();
		return new_entry_set;
	}

	inline auto get_mismatch_list()
	{
		static new_entry_list_t* mismatch_list = []()
		{
			void* raw = std::malloc(sizeof(new_entry_list_t));
			if(!raw) throw std::bad_alloc();
			return new (raw) new_entry_list_t;
		}();
		return mismatch_list;
	}

	inline void operator_delete(void* ptr, bool array_delete) noexcept
	{
		auto it = get_new_entry_set()->find(ptr);
		if(it != get_new_entry_set()->end())
		{
			if(it->is_array == array_delete)
			{
				get_new_entry_set()->erase(it);
			}
			else
			{
				try { get_mismatch_list()->push_back(*it); }
				catch(...) {}
				get_new_entry_set()->erase(it);
			}
		}
		std::free(ptr);
	}
}

namespace Rocket::Memory
{
	inline void dump_leak()
	{
		if(auto leaks = detail::get_new_entry_set(); !leaks->empty())
		{
			std::cerr << "****************************\n";
			std::cerr << "*** MEMORY LEAK(S) FOUND ***\n";
			std::cerr << "****************************\n\n";

			for(auto& entry : *leaks)
				std::cerr << entry << "\n";

			std::cerr << "\n";
		}
	}

	inline void dump_mismatch()
	{
		if(auto mismatches = detail::get_mismatch_list(); !mismatches->empty())
		{
			std::cerr << "***************************\n";
			std::cerr << "*** NEW/DELETE MISMATCH ***\n";
			std::cerr << "***************************\n\n";

			for(auto& entry : *mismatches)
				std::cerr << entry << ", freed using '" << (entry.is_array ? "delete" : "delete[]") << "'\n";

			std::cerr << "\n";
		}
	}

	inline void dump_all()
	{
		dump_leak();
		dump_mismatch();
	}
}

#ifdef ENABLE_NEW_DELETE_TRACE_DUMP
namespace { inline const struct __dump_all__ { ~__dump_all__() { Rocket::Memory::dump_all(); } } __dump_all_on_exit__; }
#endif

void* operator new (std::size_t n)
{
	void* ptr = std::malloc(n);
	if(!ptr) throw std::bad_alloc();
	return ptr;
}

void* operator new (std::size_t n, Rocket::Memory::detail::new_entry_t&& entry)
{
	void* ptr = ::operator new(n);
	entry.ptr = ptr;
	try { Rocket::Memory::detail::get_new_entry_set()->insert(std::forward<decltype(entry)>(entry)); }
	catch(...) {}
	return ptr;
}

void* operator new (std::size_t n, Rocket::Memory::detail::string_t file, int line, Rocket::Memory::detail::string_t func)
{
	return ::operator new(n, Rocket::Memory::detail::new_entry_t{ nullptr, false, n, file, line, func });
}

void* operator new [] (std::size_t n, Rocket::Memory::detail::string_t file, int line, Rocket::Memory::detail::string_t func)
{
	return ::operator new(n, Rocket::Memory::detail::new_entry_t{ nullptr, true, n, file, line, func });
}

void operator delete (void* ptr) noexcept
{
	Rocket::Memory::detail::operator_delete(ptr, false);
}

void operator delete [] (void* ptr) noexcept
{
	Rocket::Memory::detail::operator_delete(ptr, true);
}

void operator delete (void* ptr, Rocket::Memory::detail::string_t, int, Rocket::Memory::detail::string_t) noexcept
{
	Rocket::Memory::detail::operator_delete(ptr, false);
}

void operator delete [] (void* ptr, Rocket::Memory::detail::string_t, int, Rocket::Memory::detail::string_t) noexcept
{
	Rocket::Memory::detail::operator_delete(ptr, true);
}

#ifndef new
#define new new(__FILE__, __LINE__, __FUNCTION__)
#endif
