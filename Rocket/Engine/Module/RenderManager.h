#pragma once
#include "Interface/IRuntimeModule.h"
#include "Interface/IRenderLoader.h"
#include "Core/Template.h"

namespace Rocket {
    class RenderManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(RenderManager);
    public:
        virtual ~RenderManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;

        void AddRenderLoader(const Ref<IRenderLoader>& loader) { render_loader_ = loader; }
    private:
        Ref<IRenderLoader> render_loader_ = nullptr;
    };
}