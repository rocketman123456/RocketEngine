#pragma once
#include "Core/Declare.h"

namespace Rocket {
    template<typename T>
    Interface Product {
    public:
        virtual ~Product() = default;
    };

    template<typename T>
    Interface Factory {
    public:
        virtual ~Factory() = default;
        virtual T Create() = 0;
        virtual T CreateProduct() = 0;
        virtual void RegisterProduct() = 0;
    };
}
