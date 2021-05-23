#include "Module/RenderManager.h"

namespace Rocket {
    int32_t RenderManager::Initialize() {
        if(render_loader_.get() == nullptr) {
            return 1;
        }

        bool result = false; 
        result = render_loader_->LoadDevice();
        if(result) {
            RK_GRAPHICS_ERROR("Load Render Device Error");
            return result;
        }
        result = render_loader_->LoadDriver();
        if(result) {
            RK_GRAPHICS_ERROR("Load Render Driver Error");
            return result;
        }
        result = render_loader_->LoadEngine();
        if(result) {
            RK_GRAPHICS_ERROR("Load Render Engine Error");
            return result;
        }

        return 0;
    }

    void RenderManager::Finalize() {
        render_loader_->UnloadEngine();
        render_loader_->UnloadDriver();
        render_loader_->UnloadDevice();
    }

    void RenderManager::Tick(TimeStep ts) {
        
    }
}
