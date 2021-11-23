#pragma once
#include "Core/Declare.h"

#include <cstdint>
#include <vector>

// For Algorithm 4th Symbol Table
namespace Rocket {
    template<typename Key, typename Value>
    _Interface_ ST {
    public:
        virtual void put(const Key& key, const Value& value) = 0;
        virtual void remove(const Key& key) = 0;
        virtual const Value get(const Key& key) const = 0;
        virtual bool contain(const Key& key) const = 0;
        virtual bool empty() const = 0;
        virtual std::size_t size() const = 0;
        virtual std::vector<Key> keys() const = 0;
        virtual std::vector<Value> values() const = 0;
        virtual Key min() const = 0;
        virtual Key max() const = 0;
        virtual Key ceiling(const Key& key) const = 0;
        virtual Key floor(const Key& key) const = 0;
    };
}
