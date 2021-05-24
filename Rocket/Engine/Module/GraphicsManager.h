#pragma once
#include "Interface/IRuntimeModule.h"
#include "Core/Template.h"

namespace Rocket {
    class GraphicsManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(GraphicsManager);
    public:
        virtual ~GraphicsManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    };

    extern IRuntimeModule* g_GraphicsManager;
}