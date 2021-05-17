#pragma once
#include "Interface/IRuntimeModule.h"

namespace Rocket {
    class RenderManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(RenderManager);
    public:
        virtual ~RenderManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    };
}