#pragma once
#include "Interface/IRuntimeModule.h"

namespace Rocket {
    class LogicManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(LogicManager);
    public:
        virtual ~LogicManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    };

    extern IRuntimeModule* g_LogicManager;
}