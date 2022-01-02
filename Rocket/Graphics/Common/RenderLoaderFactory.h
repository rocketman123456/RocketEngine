#pragma once
#include "Common/RenderLoader.h"

namespace Rocket {
    class RenderLoaderFactory {
    public:
        static RenderLoaderPtr CreateRenderLoader();
    };
}
