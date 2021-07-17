#pragma once
#include "Core/Declare.h"

namespace Rocket {
    template<typename T>
    Interface Iterator {
    public:
        virtual bool HasNext();
        virtual T Next();
    };

    template<typename T>
    Interface Aggregate {
    public:
        Iterator<T> GetIterator();
    };
}
