#include "Memory/MemoryCheck.h"
#include "Pattern/IRuntimeModule.h"
#include "Containers/Bag/Bag.h"
#include "Log/Log.h"

using namespace Rocket;

int main() {
    Bag<IRuntimeModule*> modules;

    {
        auto iter = modules.GetIterator();
        while(iter.HasNext()) {
            IRuntimeModule* module = iter.Next();
            auto result = module->Initialize();
            if(result != 0) {
                RK_ERROR(Core, "Module {} Initialize Failed", module->ToString());
            }
        }
    }

    while(1) {
        auto iter = modules.GetIterator();
        while(iter.HasNext()) {
            IRuntimeModule* module = iter.Next();
            module->Tick(0.01);
        }
    }

    {
        auto iter = modules.GetIterator();
        while(iter.HasNext()) {
            IRuntimeModule* module = iter.Next();
            module->Finalize();
        }
    }

    return 0;
}
