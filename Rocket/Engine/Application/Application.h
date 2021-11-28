#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Pattern/Singleton.h"
#include "Pattern/IRuntimeModule.h"

#include <vector>

namespace Rocket {
    class Application : _implements_ IRuntimeModule, _implements_ AbstractSingleton<Application> {
        RUNTIME_MODULE_TYPE(Application);
    public:
        Application() = default;
        virtual ~Application() = default;

        [[nodiscard]] virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        void AddModule(IRuntimeModule* module) { modules_.emplace_back(module); }
        void RemoveModule(IRuntimeModule* module) {
            auto iter = modules_.begin();
            while(iter != modules_.end()) {
                if(*iter == module) {
                    break;
                }
                iter++;
            }
            modules_.erase(iter); 
        }

    private:
        std::vector<IRuntimeModule*> modules_;
    };
}
