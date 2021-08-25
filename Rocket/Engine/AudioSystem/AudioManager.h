#pragma once
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"
#include "FileSystem/AudioFile.h"
#include "AudioSystem/AudioTask.h"
#include "Containers/Queue/UnboundedQueue.h"
#include "Utils/Hashing.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <list>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <sndfile.h>

namespace Rocket {
    class AudioManager : implements IRuntimeModule, implements AbstractSingleton<AudioManager> {
        RUNTIME_MODULE_TYPE(AudioManager);
    public:
        AudioManager() = default;
        virtual ~AudioManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        void InsertBuffer(AudioFile* file);
        ALuint FindBuffer(const std::string& name);
        ALuint FindBuffer(const uint64_t name_id);

        void AddTask(AudioTaskPtr&& task);

    private:
        // TODO : use thread safe list
        std::list<AudioTaskPtr> tasks_;
        std::unordered_map<uint64_t, ALuint> buffers_;
        ALCdevice* openal_device_ = nullptr;
        ALCcontext* openal_context_ = nullptr;
        ALCboolean context_made_current_ = false;
    };
}
