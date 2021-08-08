#pragma once
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"

namespace Rocket {
    class FileParserManager: implements IRuntimeModule, implements AbstractSingleton<FileParserManager>  {
        RUNTIME_MODULE_TYPE(FileParserManager);
    public:
        FileParserManager() = default;
        virtual ~FileParserManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;
    };
}
