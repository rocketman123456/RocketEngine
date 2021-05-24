#pragma once
#include "Interface/IRuntimeModule.h"

namespace Rocket {
    class MemoryManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(MemoryManager);
    public:
        virtual ~MemoryManager() = default;
        
        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    };

    extern IRuntimeModule* g_MemoryManager;
}
