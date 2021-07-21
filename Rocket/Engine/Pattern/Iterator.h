#pragma once
#include "Core/Declare.h"

namespace Rocket {
    template<typename T>
    Interface Iterator {
    public:
        virtual bool HasNext() = 0;
        virtual T Next() = 0;
    };

    template<typename T>
    Interface Iterable {
    public:
        Iterator<T> GetIterator() = 0;
    };
}
