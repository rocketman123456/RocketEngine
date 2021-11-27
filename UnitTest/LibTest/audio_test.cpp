#include "Memory/MemoryCheck.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include "Utils/AudioChecker.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include <random>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <stdio.h>
#include <sndfile.h>

using namespace Rocket;

namespace std {
    enum class endian {
    #ifdef _WIN32
        little = 0,
        big    = 1,
        native = little
    #else
        little = __ORDER_LITTLE_ENDIAN__,
        big    = __ORDER_BIG_ENDIAN__,
        native = __BYTE_ORDER__
    #endif
    };
}

bool get_available_devices(std::vector<std::string>& devicesVec, ALCdevice* device) {
    const ALCchar* devices;
    if(!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
        return false;

    const char* ptr = devices;

    devicesVec.clear();

    do {
        devicesVec.push_back(std::string(ptr));
        ptr += devicesVec.back().size() + 1;
    }
    while(*(ptr + 1) != '\0');

    return true;
}

const char* FormatName(ALenum format) {
    switch(format) {
    case AL_FORMAT_MONO8: return "Mono, U8";
    case AL_FORMAT_MONO16: return "Mono, S16";
    case AL_FORMAT_MONO_FLOAT32: return "Mono, Float32";
    case AL_FORMAT_STEREO8: return "Stereo, U8";
    case AL_FORMAT_STEREO16: return "Stereo, S16";
    case AL_FORMAT_STEREO_FLOAT32: return "Stereo, Float32";
    case AL_FORMAT_BFORMAT2D_8: return "B-Format 2D, U8";
    case AL_FORMAT_BFORMAT2D_16: return "B-Format 2D, S16";
    case AL_FORMAT_BFORMAT2D_FLOAT32: return "B-Format 2D, Float32";
    case AL_FORMAT_BFORMAT3D_8: return "B-Format 3D, U8";
    case AL_FORMAT_BFORMAT3D_16: return "B-Format 3D, S16";
    case AL_FORMAT_BFORMAT3D_FLOAT32: return "B-Format 3D, Float32";
    }
    return "Unknown Format";
}

int main() {
    //Log::Init();
    std::string root = FindRootDir("_root_dir_");
    std::filesystem::path root_path = root + "\\Asset\\Music\\";
    std::filesystem::directory_iterator dir_list(root_path);
    std::vector<std::string> wav_files;
    for (auto& it:dir_list) {
        std::string name = it.path().filename().string();
        //std::cout << "File Name: " << root_path.string() + name << std::endl;
        wav_files.push_back(root_path.string() + name);
    }

    ALCdevice* openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
        return 0;
    
    std::vector<std::string> device_name;
    get_available_devices(device_name, openALDevice);
    for(auto name : device_name) {
        std::cout << "Device Name: " << name << std::endl;
    }

    ALCcontext* openALContext;
    if(!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext) {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        return 0;
    }
    ALCboolean contextMadeCurrent = false;
    if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext) 
        || contextMadeCurrent != ALC_TRUE) {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        return 0;
    }

    for(auto name : wav_files) {
        SNDFILE* file = nullptr;
        SF_INFO info;
        file = sf_open(name.c_str(), SFM_READ, &info);

        // TODO : use info.format to decode bit per sample and eiden etc.
        ALenum format = AL_NONE;
        if(info.channels == 1)
            format = AL_FORMAT_MONO16;
        else if(info.channels == 2)
            format = AL_FORMAT_STEREO16;
        else if(info.channels == 3) {
            if(sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT2D_16;
        }
        else if(info.channels == 4) {
            if(sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT3D_16;
        }

        // Decode the whole audio file to a buffer
        int16_t* data = new int16_t[info.frames * info.channels];
        sf_count_t num_frames = sf_readf_short(file, data, info.frames);
        ALsizei num_bytes = (ALsizei)(num_frames * info.channels) * (ALsizei)sizeof(int16_t);

        printf ("Opened file '%s'\n", name.c_str());
        printf ("    Sample rate : %d\n", info.samplerate);
        printf ("    Channels    : %d\n", info.channels);
        printf ("    Format      : %X\n", info.format);
        printf ("    Format Name : %s\n", FormatName(format));
        printf ("    Frames      : %d\n", (int)info.frames);
        printf ("    Sections    : %d\n", info.sections);
        printf ("    Read Frame  : %lld\n", num_frames);
        printf ("    Read Bytes  : %d\n", num_bytes);

        ALuint buffer;
        alCall(alGenBuffers, 1, &buffer);
        alCall(alBufferData, buffer, format, data, num_bytes, info.samplerate);
        delete[] data;
        sf_close(file);

        auto err = alGetError();
        if(err != AL_NO_ERROR) {
            fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
            if(buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        //std::default_random_engine rand_engine;
        //std::uniform_real_distribution<float> gen_rand(-10.0f, 10.0f);
        //float x = gen_rand(rand_engine);
        //float y = gen_rand(rand_engine);
        //float z = gen_rand(rand_engine);

        ALuint source;
        alCall(alGenSources, 1, &source);
        alCall(alSourcef, source, AL_PITCH, 1);
        alCall(alSourcef, source, AL_GAIN, 1.0f);
        alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
        //alCall(alSource3f, source, AL_POSITION, x, y, z);
        alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
        alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
        alCall(alSourcei, source, AL_BUFFER, buffer);

        assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source");

        alCall(alSourcePlay, source);

        ALint state = AL_PLAYING;
        while(state == AL_PLAYING) {
            alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        alCall(alDeleteSources, 1, &source);
        alCall(alDeleteBuffers, 1, &buffer);
    }

    alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
    alcCall(alcDestroyContext, openALDevice, openALContext);

    ALCboolean closed;
    alcCall(alcCloseDevice, closed, openALDevice, openALDevice);

    //Log::End();
    return 0;
}
