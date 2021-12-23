#pragma once
#include "ScriptSystem/Script.h"

namespace Rocket {
    class PythonScript : _implements_ Script {
    public:
        virtual ~PythonScript() = default;
    };
}
