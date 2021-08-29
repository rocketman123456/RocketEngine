#pragma once
#include "Core/Declare.h"
#include "Pattern/Singleton.h"
#include "Pattern/IRuntimeModule.h"

#include "Render/RenderDriver.h"
#include "Render/RenderAPI.h"

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
        void InstallRenderAPI(RenderAPIPtr&& api);

    private:
        std::vector<RenderDriverPtr> render_driver_;
        std::vector<RenderAPIPtr> render_api_;
    };
}
