#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

namespace Rocket {
    template<typename T>
    _Interface_ Iterator {
    public:
        virtual ~Iterator() = default;
        virtual bool HasNext() = 0;
        virtual T Next() = 0;
    };

    template<typename T, class I>
    _Interface_ Iterable {
    public:
        virtual ~Iterable() = default;
        virtual I GetIterator() = 0;
    };
}
