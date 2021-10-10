#pragma once
#include "Core/Declare.h"

namespace Rocket {
    _Interface_ Parser {
    public:
        virtual bool Parse() = 0;
    };
}
