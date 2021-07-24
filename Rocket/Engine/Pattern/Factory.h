#pragma once
#include "Core/Declare.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Rocket {
    template<class T>
    Interface Factory {
    public:
        template<typename... Args>
        std::unique_ptr<T> Create(const std::string& name, const Args &... args) {
            T* product = CreateProduct(args...);
            RegisterProduct(name, product);
            std::unique_ptr<T> ptr(product);
            return std::move(ptr);
        }
    private:
        template<typename... Args>
        T* CreateProduct(const Args &... args) { return new T(args...); }
        void RegisterProduct(const std::string& name, T* product) { register_map_[name] = product; }
    private:
        std::unordered_map<std::string, T*> register_map_;
    };
}
