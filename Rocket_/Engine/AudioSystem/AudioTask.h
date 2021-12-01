#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "FileSystem/AudioFile.h"
#include "Utils/AudioChecker.h"

#include <memory>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Rocket {
    class AudioTask {
    public:
        AudioTask() = default;
        ~AudioTask() = default;
        
        int32_t Initialize(ALuint buffer);
        void Finalize();

        void Start();
        void Restart();
        void Stop();
        void Pause();
        void Resume();

        bool IsInitial();
        bool IsPlaying();
        bool IsStoped();
        bool IsPaused();

    private:
        ALuint source_;
    };

    using AudioTaskPtr = std::unique_ptr<AudioTask>;
}