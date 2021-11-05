# 7. 引入模块划分<br>

## 7.1. IRuntimeModule<br>
`IRuntimeModule.h`
```
#pragma once
#include "Core/Declare.h"
#include "Utils/TimeStep.h"

#include <string>
#include <ostream>

namespace Rocket {
    Interface IRuntimeModule {
    public:
        virtual ~IRuntimeModule() = default;

        // Return == 0 : everything OK
        // Return != 0 : something wrong
        [[nodiscard]] virtual int Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick(TimeStep step) = 0;
        // For Debug
        [[nodiscard]] virtual inline std::string ToString() const { return GetName(); }
    protected:
        [[nodiscard]] virtual inline const char* GetName() const = 0;
    };

    inline std::ostream& operator << (std::ostream& os, const IRuntimeModule& r) {
		return os << r.ToString();
	}
}

#define RUNTIME_MODULE_TYPE(type) virtual const char* GetName() const override { return #type; }
```
## 7.2. 程序主循环<br>
以下代码仅用于展示程序运行的主流程，还未使用在实际项目中。<br>
`MainLoop.cpp`
```
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
```