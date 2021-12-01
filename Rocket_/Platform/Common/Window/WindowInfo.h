#pragma once
#include <string>
#include <cstdint>

namespace Rocket {
    typedef struct WindowInfo {
        std::string name = "";
        int32_t width = 0;
        int32_t height = 0;
        bool resizeable = false;
        bool fullscreen = false;

        WindowInfo() = default;
        ~WindowInfo() = default;
        
        WindowInfo(const WindowInfo& info) {
            this->width = info.width;
            this->height = info.height;
            this->name = info.name;
            this->resizeable = info.resizeable;
            this->fullscreen = info.fullscreen;
        }
        WindowInfo(WindowInfo&& info) {
            this->width = info.width;
            this->height = info.height;
            this->name = info.name;
            this->resizeable = info.resizeable;
            this->fullscreen = info.fullscreen;
        }
        WindowInfo& operator = (const WindowInfo& info) {
            if(this == &info)
                return *this;
            this->width = info.width;
            this->height = info.height;
            this->name = info.name;
            this->resizeable = info.resizeable;
            this->fullscreen = info.fullscreen;
            return *this;
        }
        WindowInfo& operator = (WindowInfo&& info) {
            if(this == &info)
                return *this;
            this->width = info.width;
            this->height = info.height;
            this->name = info.name;
            this->resizeable = info.resizeable;
            this->fullscreen = info.fullscreen;
            return *this;
        }
    } WindowInfo;
}
