#pragma once
#include "Interface/IRuntimeModule.h"
#include "Core/Template.h"
#include "Memory/Handle.h"

#include <utility>

namespace Rocket {
    class MemoryManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(MemoryManager);
    public:
        virtual ~MemoryManager() = default;
        
        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    private:
        Map<uint32_t, Map<uint32_t, void*>> pointer_map_;
    };

    extern IRuntimeModule* g_MemoryManager;
}
