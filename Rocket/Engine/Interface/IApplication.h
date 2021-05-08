#pragma once
#include "Interface/IRuntimeModule.h"
#include "Core/Template.h"

namespace Rocket {
    class IApplication : inheritance IRuntimeModule {
    public:
        virtual ~IApplication() = default;
        
        // Runtime Module Initialize Steps (Just For Application Module)
        virtual void PreInitializeModule() = 0; // Add Required Modules, Hook Functions, etc.
        virtual void PostInitializeModule() = 0; // Add Callback Function, etc.

        // Application Initialize Steps
        virtual void PreInitialize() = 0; // Prepare Application (Parse Command Line, etc)
        virtual void PostInitialize() = 0; // Prepare Application (Load Assets, etc)

        [[nodiscard]] virtual int InitializeModule() {
            for(auto module : modules_) {
                auto result = module->Initialize();
                if(result != 0) {
                    RK_CORE_ERROR("{} Module Initialize Failed. Error Code {}", module->ToString(), result);
                    return result;
                }
            }
            return 0;
        }

    protected:
        Vector<IRuntimeModule*> modules_;
    };
}
