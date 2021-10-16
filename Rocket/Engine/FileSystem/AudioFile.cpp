#include "FileSystem/AudioFile.h"
#include "Log/Log.h"

#include <iostream>

#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

static const char* FormatName(ALenum format) {
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

namespace Rocket {
    void AudioFile::CheckSndFileError() {
        if(sf_error((SNDFILE*)file_.file_pointer) > 0) {
            RK_ERROR(File, "Audio File Error : {}", sf_strerror((SNDFILE*)file_.file_pointer));
            throw std::runtime_error("Audio File Error");
        }
    }

    int32_t AudioFile::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = path + file_name;

        return Initialize(file_.full_name.data(), mode_);
    }

    int32_t AudioFile::Initialize(const std::string& path, FileOperateMode mode) {
        mode_ = mode;
        file_.full_name = path;

        SNDFILE* file = nullptr;
        SF_INFO* file_info = new SF_INFO;
        switch(mode) {
            case FileOperateMode::READ_BINARY: 
                file = sf_open(file_.full_name.data(), SFM_READ, file_info);
                break;
            case FileOperateMode::WRITE_BINARY: 
                file = sf_open(file_.full_name.data(), SFM_WRITE, file_info);
                break;
            case FileOperateMode::READWRITE_BINARY: 
                file = sf_open(file_.full_name.data(), SFM_RDWR, file_info);
                break;
        }

        CheckSndFileError();

        file_.file_pointer = file;
        file_.extra_file_info = file_info;

        ALenum format = AL_NONE;
        if(file_info->channels == 1) {
            format = AL_FORMAT_MONO16;
        }
        else if(file_info->channels == 2) {
            format = AL_FORMAT_STEREO16;
        }
        else if(file_info->channels == 3) {
            if(sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT2D_16;
        }
        else if(file_info->channels == 4) {
            if(sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT3D_16;
        }

        RK_TRACE(Audio, "Opened file '{}'", file_.full_name.data());
        RK_TRACE(Audio, "    Sample rate : {}", file_info->samplerate);
        RK_TRACE(Audio, "    Channels    : {}", file_info->channels);
        RK_TRACE(Audio, "    Format      : {}", file_info->format);
        RK_TRACE(Audio, "    Format Name : {}", FormatName(format));
        RK_TRACE(Audio, "    Frames      : {}", (int)file_info->frames);
        RK_TRACE(Audio, "    Sections    : {}", file_info->sections);

        return 0;
    }

    void AudioFile::Finalize() {
        sf_close((SNDFILE*)file_.file_pointer);
        if(file_.extra_file_info != nullptr)
            delete file_.extra_file_info;
        initialized_ = false;
    }

    std::size_t AudioFile::Read(AudioBuffer& buffer, std::size_t frames) {
        SF_INFO* info = (SF_INFO*)file_.extra_file_info;
        buffer.buffer = new int16_t[info->frames * info->channels];
        sf_count_t num_frames = sf_readf_short((SNDFILE*)file_.file_pointer, (int16_t*)buffer.buffer, frames);
        buffer.size = num_frames * info->channels * sizeof(int16_t);
        CheckSndFileError();
        return 0;
    }

    std::size_t AudioFile::ReadAll(AudioBuffer& buffer) {
        SF_INFO* info = (SF_INFO*)file_.extra_file_info;
        buffer.buffer = new int16_t[info->frames * info->channels];
        sf_count_t num_frames = sf_readf_short((SNDFILE*)file_.file_pointer, (int16_t*)buffer.buffer, info->frames);
        buffer.size = num_frames * info->channels * sizeof(int16_t);
        CheckSndFileError();
        return 0;
    }

    std::size_t AudioFile::Write(AudioBuffer& buffer, std::size_t frames) {
        SF_INFO* info = (SF_INFO*)file_.extra_file_info;
        sf_writef_short((SNDFILE*)file_.file_pointer, (short*)buffer.buffer, frames);
        CheckSndFileError();
        return frames;
    }

    std::size_t AudioFile::Write(AudioBuffer& buffer) {
        SF_INFO* info = (SF_INFO*)file_.extra_file_info;
        sf_writef_short((SNDFILE*)file_.file_pointer, (short*)buffer.buffer, info->frames);
        CheckSndFileError();
        return info->frames;
    }

    void AudioFile::Seek(std::size_t position) {
        auto result = sf_seek((SNDFILE*)file_.file_pointer, position, SEEK_SET);
        CheckSndFileError();
        if(result == -1) {
            RK_ERROR(File, "{} Audio File Seek {} Error", file_.full_name, position);
            throw std::runtime_error("Audio File Seek Error");
        }
    }

    void AudioFile::SeekToEnd(void) {
        auto result = sf_seek((SNDFILE*)file_.file_pointer, 0, SEEK_END);
        CheckSndFileError();
        if(result == -1) {
            RK_ERROR(File, "{} Audio File Seek End Error", file_.full_name);
            throw std::runtime_error("Audio File Seek End Error");
        }
    }

    void AudioFile::Skip(std::size_t bytes) {
        auto result = sf_seek((SNDFILE*)file_.file_pointer, bytes, SEEK_CUR);
        CheckSndFileError();
        if(result == -1) {
            RK_ERROR(File, "{} Audio File Skip {} Error", file_.full_name);
            throw std::runtime_error("Audio File Skip Error");
        }
    }
}
