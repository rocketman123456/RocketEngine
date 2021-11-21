#pragma once
#include "Core/Declare.h"

#include <cstdint>

namespace Rocket {
    template<typename Key, typename Value>
    _Interface_ ST {
    public:
        virtual void put(Key key, Value value) = 0;
        virtual Value get(Key key) = 0;
        virtual void remove(Key key) = 0;
        virtual bool contains(Key key) = 0;
        virtual bool empty() = 0;
        virtual int32_t size() = 0;
    };
}
