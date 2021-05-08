#pragma once
#include "Interface/IApplication.h"

namespace Rocket {
    class Application : inheritance IApplication { // For Test
        //RUNTIME_MODULE_TYPE(Application);
    public:
        virtual ~Application() = default;

        virtual int Initialize();
        virtual void Finalize();

        virtual void Tick(TimeStep ts);

        virtual void PreInitializeModule();
        virtual void PostInitializeModule();

        virtual void PreInitialize();
        virtual void PostInitialize();
    };
}