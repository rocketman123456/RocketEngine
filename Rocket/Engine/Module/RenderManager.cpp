#include "Module/RenderManager.h"

namespace Rocket {
    int32_t RenderManager::Initialize() {
        if(render_loader_.get() == nullptr) {
            return 1;
        }

        auto result = render_loader_->LoadDriver();
        if(result) {
            return result;
        }
        result = render_loader_->LoadDevice();
        if(result) {
            return result;
        }
        result = render_loader_->LoadEngine();
        if(result) {
            return result;
        }

        return 0;
    }

    void RenderManager::Finalize() {
        render_loader_->UnloadEngine();
        render_loader_->UnloadDevice();
        render_loader_->UnloadDriver();
    }

    void RenderManager::Tick(TimeStep ts) {
        
    }
}
