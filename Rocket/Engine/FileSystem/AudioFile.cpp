#include "FileSystem/AudioFile.h"
#include "Log/Log.h"

#ifdef RK_MEMORY_CHECK
#ifdef new
#undef new
#endif
#endif

#include <iostream>
#include <sndfile.hh>

namespace Rocket {
    void AudioFile::CheckSndFileError() {
        if(sf_error((SNDFILE*)file_.file_pointer) > 0) {
            RK_ERROR(File, "Audio File Error : {}", sf_strerror((SNDFILE*)file_.file_pointer));
        }
    }

    int32_t AudioFile::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = path + file_name;

        //SndfileHandle* file = new SndfileHandle(file_.full_name.c_str());
        SNDFILE* file = nullptr;
        SF_INFO* file_info = new SF_INFO;
        switch(mode) {
            case FileOperateMode::ReadBinary: 
                file = sf_open(file_.full_name.c_str(), SFM_READ, file_info);
                break;
            case FileOperateMode::WriteBinary: 
                file = sf_open(file_.full_name.c_str(), SFM_WRITE, file_info);
                break;
            case FileOperateMode::ReadWriteBinary: 
                file = sf_open(file_.full_name.c_str(), SFM_RDWR, file_info);
                break;
        }

        CheckSndFileError();

        file_.file_pointer = file;
        file_.extra_file_info = file_info;

        return 0;
    }

    void AudioFile::Finalize() {
        sf_close((SNDFILE*)file_.file_pointer);
        if(file_.extra_file_info != nullptr)
            delete file_.extra_file_info;
        initialized_ = false;
    }

    std::size_t AudioFile::Read(FileBuffer& buffer, std::size_t length) {
        CheckSndFileError();
        return 0;
    }

    std::size_t AudioFile::ReadAll(FileBuffer& buffer) {
        CheckSndFileError();
        return 0;
    }

    std::size_t AudioFile::Write(FileBuffer& buffer, std::size_t length) {
        CheckSndFileError();
        return 0;
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
    
    //std::size_t AudioFile::Tell(void) const {
    //    //CheckSndFileError();
    //    return 0;
    //}
}

#ifdef RK_MEMORY_CHECK
#ifndef new
#define new new(__FILE__, __LINE__, __FUNCTION__)
#endif
#endif
