#pragma once
#include "Core/Declare.h"
#include "Pattern/Singleton.h"
#include "Pattern/IRuntimeModule.h"

#include "Render/RenderDriver.h"
#include "Render/RenderAPI.h"

namespace Rocket {
    class RenderManager : public AbstractSingleton<RenderManager>, public IRuntimeModule {
        RUNTIME_MODULE_TYPE(RenderManager);
    public:
        RenderManager() = default;
        virtual ~RenderManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

    private:
        RenderDriverPtr render_driver_;
        RenderAPIPtr render_api_;
    };
}
