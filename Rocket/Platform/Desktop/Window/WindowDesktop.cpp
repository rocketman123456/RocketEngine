#include "Desktop/Window/WindowDesktop.h"

namespace Rocket {
    namespace Window {
        int32_t WindowDesktop::Initialize() {
            SDL_Init(SDL_INIT_EVERYTHING);
            window_ = SDL_CreateWindow(
                "Rocket Engine",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width_,
                height_,
#if defined(RK_VULKAN)
                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
#elif defined(RK_OPENGL)
                SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN
#elif defined(RK_METAL)
                SDL_WINDOW_METAL | SDL_WINDOW_SHOWN
#endif
            );
            return 0;
        }

        void WindowDesktop::Finalize() {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
            SDL_Quit();
        }

        void WindowDesktop::Tick(TimeStep ts) {
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    RK_CORE_INFO("App Should Quit");
                }
            }
        }
    }
}
