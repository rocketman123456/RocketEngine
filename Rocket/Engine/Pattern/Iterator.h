#pragma once
#include "Core/Declare.h"

namespace Rocket {
    template<typename T>
    Interface Iterator {
    public:
        virtual ~Iterator() = default;
        virtual bool HasNext() = 0;
        virtual T Next() = 0;
    };

    template<typename T, class I>
    Interface Iterable {
    public:
        virtual ~Iterable() = default;
        virtual I GetIterator() = 0;
    };
}
