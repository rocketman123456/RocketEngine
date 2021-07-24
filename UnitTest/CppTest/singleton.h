//https://vorbrodt.blog/2020/07/10/singleton-pattern/

#pragma once

#include <mutex>
#include <memory>
#include <utility>
#include <stdexcept>

template<typename T>
class Singleton
{
public:
	template<typename... Args>
	static void Create(Args&&... args)
	{
		static std::mutex s_lock;
		std::scoped_lock lock(s_lock);

		if(!s_instance) s_instance.reset(new T(std::forward<Args>(args)...));
		else throw std::logic_error("This singleton has already been created!");
	}

	static T* Instance() noexcept { return s_instance.get(); }

protected:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (Singleton&&) = delete;
	~Singleton() = default;

private:
	using storage_t = std::unique_ptr<T>;
	inline static storage_t s_instance = nullptr;
};

#define SINGLETON(T) final : public Singleton<T>
#define SINGLETON_CLASS(C) class C SINGLETON(C)
#define SINGLETON_STRUCT(S) struct S SINGLETON(S)
#define SINGLETON_FRIEND(T) friend class Singleton<T>

template<typename T>
class AbstractSingleton
{
public:
	template<typename... Args>
	static void Create(Args&&... args)
	{
		static std::mutex s_lock;
		std::scoped_lock lock(s_lock);

		struct Q : T
		{
			using T::T;
			void __abstract_singleton__() override {}
		};

		if(!s_instance) s_instance.reset(new Q(std::forward<Args>(args)...));
		else throw std::logic_error("This abstract singleton has already been created!");
	}

	static T* Instance() noexcept { return s_instance.get(); }

protected:
	AbstractSingleton() = default;
	AbstractSingleton(const AbstractSingleton&) = delete;
	AbstractSingleton(AbstractSingleton&&) = delete;
	AbstractSingleton& operator = (const AbstractSingleton&) = delete;
	AbstractSingleton& operator = (AbstractSingleton&&) = delete;
	virtual ~AbstractSingleton() = default;

private:
	using storage_t = std::unique_ptr<T>;
	inline static storage_t s_instance = nullptr;

	virtual void __abstract_singleton__() = 0;
};

#define ABSTRACT_SINGLETON(T) : public AbstractSingleton<T>
#define ABSTRACT_SINGLETON_CLASS(C) class C ABSTRACT_SINGLETON(C)
#define ABSTRACT_SINGLETON_STRUCT(S) struct S ABSTRACT_SINGLETON(S)
#define ABSTRACT_SINGLETON_FRIEND(T) friend class AbstractSingleton<T>
