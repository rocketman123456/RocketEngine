//https://vorbrodt.blog/2020/07/10/singleton-pattern/
#pragma once
#include "Core/MemoryDefine.h"
#include "Core/Declare.h"

#include <mutex>
#include <memory>
#include <utility>
#include <stdexcept>

namespace Rocket {
    template<typename T>
    _Interface_ Singleton {
    public:
        // Should Create before use
        template<typename... Args>
        static void Create(Args&&... args) {
            std::call_once(flag, [&](){ s_instance.reset(new T(std::forward<Args>(args)...)); });
        }

        static T* Instance() noexcept { return s_instance.get(); }

        // Can Destroy after use
        static void Destroy() {
            s_instance.reset();
        }

    protected:
        Singleton() = default;
        ~Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator = (const Singleton&) = delete;
        Singleton& operator = (Singleton&&) = delete;

    private:
        using storage_t = std::unique_ptr<T>;
        inline static storage_t s_instance = nullptr;
        inline static std::once_flag flag;
    };

    template<typename T>
    _Interface_ AbstractSingleton {
    public:
        // Should Create before use
        template<typename... Args>
        static void Create(Args&&... args) {
            struct Q : T {
                using T::T;
                void __abstract_singleton__() override {}
            };

            std::call_once(flag, [&](){ s_instance = std::make_unique<Q>(args...); });
        }

        // Can Destroy after use
        static void Destroy() {
            s_instance.reset();
        }

        static T* Instance() noexcept { return s_instance.get(); }

    protected:
        AbstractSingleton() = default;
        virtual ~AbstractSingleton() = default;
        AbstractSingleton(const AbstractSingleton&) = delete;
        AbstractSingleton(AbstractSingleton&&) = delete;
        AbstractSingleton& operator = (const AbstractSingleton&) = delete;
        AbstractSingleton& operator = (AbstractSingleton&&) = delete;

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
