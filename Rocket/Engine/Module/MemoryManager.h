#pragma once
#include "Interface/IRuntimeModule.h"
#include "Memory/Handle.h"

#include <utility>
#include <map>

namespace Rocket {
    class MemoryManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(MemoryManager);
    public:
        virtual ~MemoryManager() = default;
        
        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    private:
        std::map<uint32_t, std::map<uint32_t, void*>> pointer_map_;
    };

    extern IRuntimeModule* g_MemoryManager;
}
