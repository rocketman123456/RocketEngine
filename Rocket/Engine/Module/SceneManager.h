#pragma once
#include "Interface/IRuntimeModule.h"

namespace Rocket {
    class SceneManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(SceneManager);
    public:
        virtual ~SceneManager() = default;
        
        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;
    };
}
