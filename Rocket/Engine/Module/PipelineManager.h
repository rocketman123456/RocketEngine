#pragma once
#include "Interface/IRuntimeModule.h"
#include "Core/Template.h"

namespace Rocket {
    class PipelineManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(PipelineManager);
    public:
        virtual ~PipelineManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    };

    extern IRuntimeModule* g_PipelineManager;
}