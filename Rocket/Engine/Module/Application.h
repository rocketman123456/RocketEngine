#pragma once
#include "Interface/IApplication.h"

namespace Rocket {
    class Application : inheritance IApplication { // For Test
        //RUNTIME_MODULE_TYPE(Application);
    public:
        virtual ~Application() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;

        virtual void Tick(TimeStep ts);

        virtual void PreInitializeModule();
        virtual void PostInitializeModule();

        virtual void PreInitialize();
        virtual void PostInitialize();
    };

    extern IApplication* g_Application;
}