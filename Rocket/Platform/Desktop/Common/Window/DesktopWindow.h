#pragma once
#include "Core/Declare.h"
#include "Window/WindowInstance.h"

namespace Rocket {
    class DesktopWindow : _implements_ WindowInstance {
    public:
        DesktopWindow() = default;
        virtual ~DesktopWindow() = default;

        virtual int32_t Initialize(const WindowInfo& info) final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep dt) final;
        virtual bool GetShouldClose() final;
    };
}