#pragma once
#include "Core/Declare.h"
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"
#include "MultiThread/Task.h"
#include "MultiThread/ThreadPool.h"


namespace Rocket {
    class TaskManager : _implements_ IRuntimeModule, _implements_ AbstractSingleton<TaskManager> {
        RUNTIME_MODULE_TYPE(TaskManager);
    public:
        TaskManager() = default;
        virtual ~TaskManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;
    private:
        ThreadPool pool;
    };
}
