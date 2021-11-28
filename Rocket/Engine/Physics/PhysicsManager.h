#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Pattern/Singleton.h"
#include "Pattern/IRuntimeModule.h"

namespace Rocket {
    class PhysicsManager : _implements_ IRuntimeModule, _implements_ AbstractSingleton<PhysicsManager> {
        RUNTIME_MODULE_TYPE(PhysicsManager);
    public:
        PhysicsManager() = default;
        virtual ~PhysicsManager() = default;

        [[nodiscard]] virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;
    };
}
