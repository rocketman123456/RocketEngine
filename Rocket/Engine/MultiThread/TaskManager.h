#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"
#include "MultiThread/Task.h"
#include "MultiThread/ThreadPool.h"

#include <memory>


namespace Rocket {
    class TaskManager : _implements_ IRuntimeModule, _implements_ AbstractSingleton<TaskManager> {
        RUNTIME_MODULE_TYPE(TaskManager);
    public:
        TaskManager() : pool_(std::make_unique<ThreadPool>(2)) {}
        virtual ~TaskManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        std::unique_ptr<ThreadPool>& GetThreadPool() { return pool_; }
    private:
        std::unique_ptr<ThreadPool> pool_;
    };
}
