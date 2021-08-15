//https://vorbrodt.blog/2020/07/10/singleton-pattern/
#pragma once
#include "Core/Declare.h"

#include <mutex>
#include <memory>
#include <utility>
#include <stdexcept>

namespace Rocket {
    template<typename T>
    Interface Singleton {
    public:
        template<typename... Args>
        static void Create(Args&&... args) {
            //static std::mutex s_lock;
            //std::scoped_lock lock(s_lock);
            std::call_once(flag, [&](){ s_instance.reset(new T(std::forward<Args>(args)...)); });
            //if(!s_instance) s_instance.reset(new T(std::forward<Args>(args)...));
            //else throw std::logic_error("This singleton has already been created!");
        }

        static T* Instance() noexcept { return s_instance.get(); }

        static void Destroy() {
            s_instance.reset();
        }

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
        inline static std::once_flag flag;
    };

    template<typename T>
    Interface AbstractSingleton {
    public:
        template<typename... Args>
        static void Create(Args&&... args) {
            //static std::mutex s_lock;
            //std::scoped_lock lock(s_lock);

            struct Q : T {
                using T::T;
                void __abstract_singleton__() override {}
            };

            std::call_once(flag, [&](){ s_instance = std::make_unique<Q>(args...); });
            //if(!s_instance) s_instance.reset(new Q(std::forward<Args>(args)...));
            //else throw std::logic_error("This abstract singleton has already been created!");
        }

        static void Destroy() {
            s_instance.reset();
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
        inline static std::once_flag flag;

        virtual void __abstract_singleton__() = 0;
    };
}

#define SINGLETON(T) final : public Rocket::Singleton<T>
#define SINGLETON_CLASS(C) class C SINGLETON(C)
#define SINGLETON_STRUCT(S) struct S SINGLETON(S)
#define SINGLETON_FRIEND(T) friend class Rocket::Singleton<T>

#define ABSTRACT_SINGLETON(T) : public Rocket::AbstractSingleton<T>
#define ABSTRACT_SINGLETON_CLASS(C) class C ABSTRACT_SINGLETON(C)
#define ABSTRACT_SINGLETON_STRUCT(S) struct S ABSTRACT_SINGLETON(S)
#define ABSTRACT_SINGLETON_FRIEND(T) friend class Rocket::AbstractSingleton<T>
