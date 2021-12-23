#pragma once
#include "ScriptSystem/Script.h"

namespace Rocket {
    class LuaScript : _implements_ Script {
    public:
        virtual ~LuaScript() = default;
    };
}
