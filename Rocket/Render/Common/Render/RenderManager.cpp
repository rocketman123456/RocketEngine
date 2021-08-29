#include "Render/RenderManager.h"

namespace Rocket {
    int32_t RenderManager::Initialize() {
        return 0;
    }

    void RenderManager::Finalize() {
    }

    void RenderManager::Tick(TimeStep step) {
    }

    void RenderManager::InstallRenderDriver(RenderDriverPtr&& driver) {
        render_driver_.emplace_back(std::move(driver));
    }

    void RenderManager::InstallRenderAPI(RenderAPIPtr&& api) {
        render_api_.emplace_back(std::move(api));
    }
}
