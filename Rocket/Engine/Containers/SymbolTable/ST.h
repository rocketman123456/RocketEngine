#pragma once
#include "Core/Declare.h"

#include <cstdint>
#include <vector>

namespace Rocket {
    template<typename Key, typename Value>
    _Interface_ ST {
    public:
        virtual void put(Key key, Value value) = 0;
        virtual void remove(const Key& key) = 0;
        virtual const Value get(const Key& key) const = 0;
        virtual bool contain(const Key& key) const = 0;
        virtual bool empty() const = 0;
        virtual std::size_t size() const = 0;
        virtual const std::vector<Key> keys() const = 0;
    };
}
