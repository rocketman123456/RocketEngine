#pragma once
#include "Containers/SymbolTable/ST.h"


namespace Rocket {
    template<typename S, typename T>
    class SeparateChainingHashST : _implements_ ST<S, T> {
        virtual void put(const Key& key, const Value& value) = 0;
        virtual void remove(const Key& key) = 0;
        virtual const Value get(const Key& key) const = 0;
        virtual bool contain(const Key& key) const = 0;
        virtual bool empty() const = 0;
        virtual std::size_t size() const = 0;
        virtual const std::vector<Key> keys() const = 0;
    };
}