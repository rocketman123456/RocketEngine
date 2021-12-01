#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Pattern/Singleton.h"
#include "Pattern/IRuntimeModule.h"

#include "Render/RenderDriver.h"

#include <vector>

namespace Rocket {
    class RenderManager : public AbstractSingleton<RenderManager>, public IRuntimeModule {
        RUNTIME_MODULE_TYPE(RenderManager);
    public:
        RenderManager() = default;
        virtual ~RenderManager() = default;

        [[nodiscard]] virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        void InstallRenderDriver(RenderDriverPtr&& driver);

    private:
        std::vector<RenderDriverPtr> render_driver_;
    };
}
