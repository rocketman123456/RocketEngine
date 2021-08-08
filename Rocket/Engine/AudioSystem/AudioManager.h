#pragma once
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"

namespace Rocket {
    class AudioManager : implements IRuntimeModule, implements AbstractSingleton<AudioManager> {
        RUNTIME_MODULE_TYPE(AudioManager);
    public:
        AudioManager() = default;
        virtual ~AudioManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;
    };
}
