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
#include <bit>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

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

std::int32_t convert_to_int(char* buffer, std::size_t len)
{
    std::int32_t a = 0;
    if(std::endian::native == std::endian::little)
        std::memcpy(&a, buffer, len);
    else
        for(std::size_t i = 0; i < len; ++i)
            reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
    return a;
}

bool load_wav_file_header(
    std::ifstream& file,
    std::uint8_t& channels,
    std::int32_t& sampleRate,
    std::uint8_t& bitsPerSample,
    ALsizei& size)
{
    char buffer[4];
    if(!file.is_open())
        return false;

    // the RIFF
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
        return false;
    }
    if(std::strncmp(buffer, "RIFF", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
        return false;
    }

    // the size of the file
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read size of file" << std::endl;
        return false;
    }

    // the WAVE
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        return false;
    }
    if(std::strncmp(buffer, "WAVE", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
        return false;
    }

    // "fmt/0"
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        return false;
    }

    // PCM should be 1?
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        return false;
    }

    // the number of channels
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        return false;
    }
    channels = convert_to_int(buffer, 2);

    // sample rate
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        return false;
    }
    sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        return false;
    }

    // ?? dafaq
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        return false;
    }

    // bitsPerSample
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        return false;
    }
    bitsPerSample = convert_to_int(buffer, 2);
    
    // data chunk header "data"
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data chunk header" << std::endl;
        return false;
    }
    if(std::strncmp(buffer, "data", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        return false;
    }

    // size of data
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data size" << std::endl;
        return false;
    }
    size = convert_to_int(buffer, 4);

    /* cannot be at the end of file */
    if(file.eof())
    {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
        return false;
    }
    if(file.fail())
    {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
        return false;
    }

    return true;
}

bool load_wav(
    const std::string& filename,
    std::uint8_t& channels,
    std::int32_t& sampleRate,
    std::uint8_t& bitsPerSample,
    std::vector<char> data)
{
    ALsizei size = 0;
    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open())
    {
        std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
        return false;
    }
    if(!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
    {
        std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
        return false;
    }

    data.resize(size);
    in.read(data.data(), size);

    return true;
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

int main() {
    Log::Init();
    std::string root = FindRootDir("_root_dir_");
    std::filesystem::path root_path = root + "/Asset/Music/";
    std::filesystem::directory_iterator dir_list(root_path);
    std::vector<std::string> wav_files;
    for (auto& it:dir_list) {
        std::string name = it.path().filename().string();
        std::cout << "File Name: " << root_path.string() + name << std::endl;
        wav_files.push_back(root_path.string() + name);
    }

    ALCdevice* openALDevice = alcOpenDevice(nullptr);
    if(!openALDevice)
        return 0;

    ALCcontext* openALContext;
    if(!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext)
    {
        std::cerr << "ERROR: Could not create audio context" << std::endl;
        return 0;
    }
    ALCboolean contextMadeCurrent = false;
    if(!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext) 
        || contextMadeCurrent != ALC_TRUE)
    {
        std::cerr << "ERROR: Could not make audio context current" << std::endl;
        return 0;
    }

    std::uint8_t channels;
    std::int32_t sampleRate;
    std::uint8_t bitsPerSample;
    std::vector<char> soundData;
    if(!load_wav(wav_files[0], channels, sampleRate, bitsPerSample, soundData))
    {
        std::cerr << "ERROR: Could not load wav" << std::endl;
        return 0;
    }

    ALuint buffer;
    alCall(alGenBuffers, 1, &buffer);

    ALenum format;
    if(channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if(channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if(channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if(channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr
            << "ERROR: unrecognised wave format: "
            << channels << " channels, "
            << bitsPerSample << " bps" << std::endl;
        return 0;
    }

    alCall(alBufferData, buffer, format, soundData.data(), soundData.size(), sampleRate);
    soundData.clear(); // erase the sound in RAM

    ALuint source;
    alCall(alGenSources, 1, &source);
    alCall(alSourcef, source, AL_PITCH, 1);
    alCall(alSourcef, source, AL_GAIN, 1.0f);
    alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
    alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
    alCall(alSourcei, source, AL_BUFFER, buffer);

    alCall(alSourcePlay, source);

    ALint state = AL_PLAYING;

    while(state == AL_PLAYING)
    {
        alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    }

    alCall(alDeleteSources, 1, &source);
    alCall(alDeleteBuffers, 1, &buffer);

    alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
    alcCall(alcDestroyContext, openALDevice, openALContext);

    ALCboolean closed;
    alcCall(alcCloseDevice, closed, openALDevice, openALDevice);

    Log::End();
    return 0;
}
