#include "FileSystem/AudioFile.h"

#ifdef RK_MEMORY_CHECK
#ifdef new
#undef new
#endif
#endif

#include <iostream>
#include <sndfile.hh>

bool CheckALErrors(const std::string& filename, const std::uint_fast32_t line) {
    ALenum error = alGetError();
    if(error != AL_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error) {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

bool CheckALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device) {
    ALCenum error = alcGetError(device);
    if(error != ALC_NO_ERROR) {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error) {
        case ALC_INVALID_VALUE:
            std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case ALC_INVALID_DEVICE:
            std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            break;
        case ALC_INVALID_CONTEXT:
            std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            break;
        case ALC_INVALID_ENUM:
            std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            break;
        case ALC_OUT_OF_MEMORY:
            std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
            break;
        default:
            std::cerr << "UNKNOWN ALC ERROR: " << error;
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

namespace Rocket {
    int32_t AudioFile::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = path + file_name;

        //SndfileHandle* file = new SndfileHandle(file_.full_name.c_str());
        SNDFILE* file = nullptr;
        SF_INFO* file_info = new SF_INFO;
        switch(mode) {
            case FileOperateMode::ReadBinary: {
                file = sf_open(file_.full_name.c_str(), SFM_READ, file_info);
                break;
            }
            case FileOperateMode::WriteBinary: {
                file = sf_open(file_.full_name.c_str(), SFM_WRITE, file_info);
                break;
            }
            case FileOperateMode::ReadWriteBinary: {
                file = sf_open(file_.full_name.c_str(), SFM_RDWR, file_info);
                break;
            }
        }

        file_.file_pointer = file;

        return 0;
    }

    void AudioFile::Finalize() {
        sf_close((SNDFILE*)file_.file_pointer);
    }

    std::size_t AudioFile::Read(FileBuffer& buffer, std::size_t length) {
        return 0;
    }

    std::size_t AudioFile::ReadAll(FileBuffer& buffer) {
        return 0;
    }

    std::size_t AudioFile::Write(FileBuffer& buffer, std::size_t length) {
        return 0;
    }
}

#ifdef RK_MEMORY_CHECK
#ifndef new
#define new new(__FILE__, __LINE__, __FUNCTION__)
#endif
#endif
