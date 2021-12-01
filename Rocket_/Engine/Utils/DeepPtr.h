#pragma once

#include <memory>
#include <utility>
#include <compare>
#include <type_traits>
#include <initializer_list>
#include <cstddef>

// Not Useable Until c++20

namespace Rocket {
	template<typename T>
	struct DefaultCloner {
		using pointer = T*;
		pointer operator () (pointer p) const {
			return (p ? new T{ *p } : nullptr);
		}
	};

	template<typename T, typename C = DefaultCloner<T>, typename D = std::default_delete<T>>
	class DeepPtr {
	public:
		using pointer = T*;
		using element_type = T;
		using reference_type = T&;
		using cloner_type = C;
		using deleter_type = D;

		constexpr DeepPtr() noexcept = default;
		constexpr DeepPtr(std::nullptr_t) noexcept {}

		explicit DeepPtr(pointer p) noexcept : m_p{ p } {}

		DeepPtr(pointer p, cloner_type c) : m_c{ c }, m_p{ p } {}
		DeepPtr(pointer p, deleter_type d) : m_d{ d }, m_p{ p } {}
		DeepPtr(pointer p, cloner_type c, deleter_type d) : m_c{ c }, m_d{ d }, m_p{ p } {}

		DeepPtr(const DeepPtr& d) : m_c{ d.m_c }, m_d{ d.m_d }, m_p{ get_cloner()(d.m_p) } {}
		DeepPtr(DeepPtr&& d) noexcept : m_c{ std::move(d.m_c) }, m_d{ std::move(d.m_d) }, m_p{ std::exchange(d.m_p, nullptr) } {}

		template<typename U, typename V, typename W>
		DeepPtr(const DeepPtr<U, V, W>& d) : m_c{ d.m_c }, m_d{ d.m_d }, m_p{ get_cloner()(d.m_p) } {}

		template<typename U, typename V, typename W>
		DeepPtr(DeepPtr<U, V, W>&& d) noexcept : m_c{ std::move(d.m_c) }, m_d{ std::move(d.m_d) }, m_p{ std::exchange(d.m_p, nullptr) } {}

		~DeepPtr() noexcept { get_deleter()(get()); }

		DeepPtr& operator = (DeepPtr r) noexcept {
			swap(r);
			return *this;
		}

		template<typename U, typename V, typename W>
		friend bool operator == (const DeepPtr<T, C, D>& x, const DeepPtr<U, V, W>& y) noexcept { return x.m_p == y.m_p; }

		template<typename U, typename V, typename W>
		friend auto operator <=> (const DeepPtr<T, D, D>& x, const DeepPtr<U, V, W>& y) noexcept { return x.m_p <=> y.m_p; }

		explicit operator bool () const noexcept { return m_p != nullptr; }

		reference_type operator * () const { return *m_p; }

		pointer operator -> () const noexcept { return m_p; }

		pointer get() const noexcept { return m_p; }

		deleter_type& get_deleter() noexcept { return m_d; }

		const deleter_type& get_deleter() const noexcept { return m_d; }

		cloner_type& get_cloner() noexcept { return m_c; }

		const cloner_type& get_cloner() const noexcept { return m_c; }

		pointer release() noexcept { return std::exchange(m_p, nullptr); }

		void reset(pointer p = pointer()) noexcept { get_deleter()(std::exchange(m_p, p)); }

		void swap(DeepPtr& o) noexcept {
			std::swap(m_c, o.m_c);
			std::swap(m_d, o.m_d);
			std::swap(m_p, o.m_p);
		}

	private:
		template<typename, typename, typename> friend class DeepPtr;

		cloner_type m_c = cloner_type();
		deleter_type m_d = deleter_type();
		pointer m_p = pointer();
	};

	template<typename T, typename C, typename D>
	inline void swap(DeepPtr<T, C, D>& x, DeepPtr<T, C, D>& y) { x.swap(y); }

	template<typename T, typename V>
	inline auto make_deep(std::initializer_list<V> l) {
		using U = std::decay_t<T>;
		return DeepPtr<T>(new U(l));
	}

	template<typename T, typename... A>
	inline auto make_deep(A&&... a) {
		return DeepPtr<T>(new T(std::forward<A>(a)...));
	}
}
