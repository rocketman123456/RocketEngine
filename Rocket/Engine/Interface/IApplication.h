#pragma once
#include "Interface/IRuntimeModule.h"
#include "Core/Template.h"

namespace Rocket {
    class IApplication : inheritance IRuntimeModule {
    public:
        virtual ~IApplication() = default;

        // Runtime Module Initialize Steps (Just For Application Module)
        [[noreturn]] virtual void PreInitializeModule() = 0; // Add Required Modules, Hook Functions, etc.
        [[noreturn]] virtual void PostInitializeModule() = 0; // Add Callback Function, etc.

        // Application Initialize Steps
        virtual void PreInitialize() = 0; // Prepare Application (Load Basic Parameters, etc)
        virtual void PostInitialize() = 0; // Prepare Application (Load Assets, etc)

        [[nodiscard]] virtual int32_t InitializeModule() {
            for(auto module : modules_) {
                auto result = module->Initialize();
                if(result != 0) {
                    RK_CORE_ERROR("{} Module Initialize Failed. Error Code {}", module->ToString(), result);
                    return result;
                }
            }
            RK_TRACE("Application::InitializeModule");
            return 0;
        }

        void FinalizeModule() {
            for(auto module : modules_) {
                module->Finalize();
            }
            RK_TRACE("Application::FinalizeModule");
        }

        inline void AddModule(IRuntimeModule* module) { modules_.push_back(module); }
        inline bool GetAppRuning() { return app_running_; }

    protected:
        Vec<IRuntimeModule*> modules_;
        bool app_running_ = true;
    };
}
