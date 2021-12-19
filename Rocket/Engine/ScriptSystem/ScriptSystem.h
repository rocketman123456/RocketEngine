#pragma once
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"

namespace Rocket {
    class ScriptSystem : _implements_ IRuntimeModule, AbstractSingleton<ScriptSystem> {
        RUNTIME_MODULE_TYPE(ScriptSystem);
    public:
        void Initialize() final;
        void Finalize() final;
        void Tick(TimeStep step) final;
    };
}
