#include "AudioSystem/AudioTask.h"
#include "Log/Log.h"

#include <stdexcept>

namespace Rocket {
    int32_t AudioTask::Initialize(ALuint buffer) {
        alCall(alGenSources, 1, &source_);
        alCall(alSourcef, source_, AL_PITCH, 1);
        alCall(alSourcef, source_, AL_GAIN, 1.0f);
        alCall(alSource3f, source_, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, source_, AL_VELOCITY, 0, 0, 0);
        alCall(alSourcei, source_, AL_LOOPING, AL_FALSE);
        alCall(alSourcei, source_, AL_BUFFER, buffer);

        if(alGetError() != AL_NO_ERROR) {
            RK_ERROR(Audio, "Failed to setup sound source");
            throw std::exception("Failed to setup sound source");
        }
        return 0;
    }

    void AudioTask::Finalize() {
        if(!IsStoped())
            alSourceStop(source_);
        alCall(alDeleteSources, 1, &source_);
    }

    void AudioTask::Start() {
        alCall(alSourcePlay, source_);
    }

    void AudioTask::Restart() {
        alCall(alSourceRewind, source_);
    }

    void AudioTask::Stop() {
        alCall(alSourceStop, source_);
    }

    void AudioTask::Pause() {
        alCall(alSourcePause, source_);
    }

    void AudioTask::Resume() {
        alCall(alSourcePlay, source_);
    }

    bool AudioTask::IsInitial() {
        ALint state = AL_NONE;
        alCall(alGetSourcei, source_, AL_SOURCE_STATE, &state);
        return state == AL_INITIAL;
    }

    bool AudioTask::IsPlaying() {
        ALint state = AL_NONE;
        alCall(alGetSourcei, source_, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    bool AudioTask::IsStoped() {
        ALint state = AL_NONE;
        alCall(alGetSourcei, source_, AL_SOURCE_STATE, &state);
        return state == AL_STOPPED;
    }

    bool AudioTask::IsPaused() {
        ALint state = AL_NONE;
        alCall(alGetSourcei, source_, AL_SOURCE_STATE, &state);
        return state == AL_PAUSED;
    }
}
