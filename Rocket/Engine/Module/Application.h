#pragma once
#include "Interface/IApplication.h"

namespace Rocket {
    class Application : implements IApplication { // For Test
        RUNTIME_MODULE_TYPE(Application);
    public:
        virtual int Initialize() final;
        virtual void Finalize() final;

        virtual void Tick(TimeStep ts) final;

        virtual void PreInitializeModule() final;
        virtual void PostInitializeModule() final;

        virtual void PreInitialize() final;
        virtual void PostInitialize() final;
    };
}