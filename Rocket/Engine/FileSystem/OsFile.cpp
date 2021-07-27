#include "FileSystem/OsFile.h"
#include "Log/Log.h"

#include <exception>

namespace Rocket {
    int32_t OsFile::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_path_ = path;
        file_name_ = file_name;
        full_name_ = file_path_ + file_name_;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = full_name_;

        switch(mode_) {
            case FileOperateMode::ReadBinary: file_.file = fopen(full_name_.c_str(), "rb"); break;
            case FileOperateMode::WriteBinary: file_.file = fopen(full_name_.c_str(), "wb"); break;
            case FileOperateMode::ReadText: file_.file = fopen(full_name_.c_str(), "r"); break;
            case FileOperateMode::WriteText: file_.file = fopen(full_name_.c_str(), "w"); break;
            default: break;
        };

        SeekToEnd();
        file_.total_size = Tell();
        Seek(0);

        initialized_ = true;

        RK_TRACE(File, "Open File {} Success", full_name_);

        if(file_.file == nullptr)
            return 1;
        else
            return 0;
    }

    void OsFile::Finalize() {
        if(file_.file != nullptr)
            fclose(file_.file);
        initialized_ = false;
    }

    std::size_t OsFile::Read(FileBuffer& buffer, std::size_t length) {
        if(mode_ == FileOperateMode::ReadBinary) {
            buffer.size = length;
            buffer.buffer = new int8_t[length];
            auto result = fread(buffer.buffer, length, 1, file_.file);
            if(result == 1)
                return length;
            else
                return 0;
        }
        else if(mode_ == FileOperateMode::ReadText) {
            buffer.size = length + 1;
            buffer.buffer = new int8_t[length + 1];
            auto result = fread(buffer.buffer, length, 1, file_.file);
            static_cast<char*>(buffer.buffer)[length] = '\0';
            if(result == 1)
                return 0;
            else
                return length;
        }
        else {
            return 0;
        }
    }

    std::size_t OsFile::ReadAll(FileBuffer& buffer) {
        if(mode_ == FileOperateMode::ReadBinary || mode_ == FileOperateMode::ReadText)
            return Read(buffer, file_.total_size);
        else 
            return 0;
    }

    std::size_t OsFile::Write(FileBuffer& buffer, std::size_t length) {
        std::size_t real_length = 0;
        if(length == 0 || length > buffer.size)
            real_length = buffer.size;
        else
            real_length = length;
        
        auto result = fwrite(buffer.buffer, real_length, 1, file_.file);
        if(result == 1)
            return length;
        else
            return 0;
    }

    void OsFile::Seek(std::size_t position) {
        auto result = fseek(file_.file, position, SEEK_SET);
        if(result != 0) {
            RK_ERROR(File, "{} File Seek {} Error", file_.full_name, position);
            throw std::runtime_error("File Seek Error");
        }
    }

    void OsFile::SeekToEnd(void) {
        auto result = fseek(file_.file, 0, SEEK_END);
        if(result != 0) {
            RK_ERROR(File, "{} File Seek End Error", file_.full_name);
            throw std::runtime_error("File Seek End Error");
        }
    }

    void OsFile::Skip(std::size_t bytes) {
        auto result = fseek(file_.file, bytes, SEEK_CUR);
        if(result != 0) {
            RK_ERROR(File, "{} File Skip {} Error", file_.full_name);
            throw std::runtime_error("File Skip Error");
        }
    }

    std::size_t OsFile::Tell(void) const {
        std::size_t result = ftell(file_.file);
        return result;
    }
}
