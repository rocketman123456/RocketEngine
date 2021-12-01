#include "AudioSystem/AudioManager.h"
#include "Log/Log.h"

namespace Rocket {
    int32_t AudioManager::Initialize() {
        openal_device_ = alcOpenDevice(nullptr);
        if(!openal_device_) {
            RK_ERROR(Audio, "ERROR: Failed to open audio device");
            throw std::runtime_error("ERROR: Failed to open audio device");
        }
        if(!alcCall(alcCreateContext, openal_context_, openal_device_, openal_device_, nullptr) || !openal_context_) {
            RK_ERROR(Audio, "ERROR: Could not create audio context");
            throw std::runtime_error("ERROR: Could not create audio context");
        }
        if(!alcCall(alcMakeContextCurrent, context_made_current_, openal_device_, openal_context_) || context_made_current_ != ALC_TRUE) {
            RK_ERROR(Audio, "ERROR: Could not make audio context current");
            throw std::runtime_error("ERROR: Could not make audio context current");
        }
        return 0;
    }

    void AudioManager::Finalize() {
        for(auto it = tasks_.begin(); it != tasks_.end(); ++it) {
            (*it)->Stop();
            (*it)->Finalize();
        }
        ALCboolean closed;
        alcCall(alcMakeContextCurrent, context_made_current_, openal_device_, nullptr);
        alcCall(alcDestroyContext, openal_device_, openal_context_);
        alcCall(alcCloseDevice, closed, openal_device_, openal_device_);
    }

    void AudioManager::Tick(TimeStep step) {
        for(auto it = tasks_.begin(); it != tasks_.end(); ++it) {
            if((*it)->IsStoped()) {
                (*it)->Finalize();
                tasks_.erase(it);
            }
        }
    }

    void AudioManager::InsertBuffer(AudioFile* file) {
        std::string name = file->GetFileHandle()->file_name.data();
        auto hash_name = hash(name);

        auto result = buffers_.find(hash_name);
        if(result == buffers_.end()) {
            SNDFILE* file_ptr = (SNDFILE*)file->GetFileHandle()->file_pointer;
            SF_INFO* file_info = (SF_INFO*)file->GetFileHandle()->extra_file_info;

            ALenum format = AL_NONE;
            if(file_info->channels == 1)
                format = AL_FORMAT_MONO16;
            else if(file_info->channels == 2)
                format = AL_FORMAT_STEREO16;
            else if(file_info->channels == 3) {
                if(sf_command(file_ptr, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
                    format = AL_FORMAT_BFORMAT2D_16;
            }
            else if(file_info->channels == 4) {
                if(sf_command(file_ptr, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
                    format = AL_FORMAT_BFORMAT3D_16;
            }

            AudioBuffer buffer;
            file->ReadAll(buffer);
            ALuint audio_buffer;
            alCall(alGenBuffers, 1, &audio_buffer);
            alCall(alBufferData, audio_buffer, format, buffer.buffer, buffer.size, file_info->samplerate);

            auto err = alGetError();
            if(err != AL_NO_ERROR) {
                RK_ERROR(Audio, "OpenAL Error: {}", alGetString(err));
                if(audio_buffer && alIsBuffer(audio_buffer))
                    alDeleteBuffers(1, &audio_buffer);
                return;
            }

            buffers_[hash_name] = audio_buffer;
        }
    }

    ALuint AudioManager::FindBuffer(const std::string& name) {
        auto hash_name = hash(name);

        auto result = buffers_.find(hash_name);
        if(result != buffers_.end()) {
            RK_TRACE(Audio, "Find Buffer: {}", name);
            return result->second;
        } else {
            RK_TRACE(Audio, "Cannot Find Buffer: {}", name);
            return 0;
        }
    }

    ALuint AudioManager::FindBuffer(const uint64_t name_id) {
        auto result = buffers_.find(name_id);
        if(result != buffers_.end()) {
            RK_TRACE(Audio, "Find Buffer: {}", name_id);
            return result->second;
        } else {
            RK_TRACE(Audio, "Cannot Find Buffer: {}", name_id);
            return 0;
        }
    }

    void AudioManager::AddTask(AudioTaskPtr&& task) {
        task->Start();
        tasks_.push_back(std::move(task));
    }
} 
