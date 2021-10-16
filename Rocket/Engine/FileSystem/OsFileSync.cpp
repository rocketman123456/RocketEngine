#include "FileSystem/OsFileSync.h"
#include "Log/Log.h"

#include <exception>

namespace Rocket {
    int32_t OsFileSync::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = path + file_name;

        return Initialize(file_.full_name.data(), mode_);
    }

    int32_t OsFileSync::Initialize(const std::string& path, FileOperateMode mode) {
        mode_ = mode;
        file_.full_name = path;

        switch(mode_) {
            case FileOperateMode::READ_BINARY: file_.file_pointer = (void*)fopen(file_.full_name.data(), "rb"); break;
            case FileOperateMode::WRITE_BINARY: file_.file_pointer = (void*)fopen(file_.full_name.data(), "wb"); break;
            case FileOperateMode::READ_TEXT: file_.file_pointer = (void*)fopen(file_.full_name.data(), "r"); break;
            case FileOperateMode::WRITE_TEXT: file_.file_pointer = (void*)fopen(file_.full_name.data(), "w"); break;
            default: break;
        };

        SeekToEnd();
        file_.total_size = Tell();
        Seek(0);

        initialized_ = true;

        RK_TRACE(File, "Open File {} Success", file_.full_name);

        if(file_.file_pointer == nullptr)
            return 1;
        else
            return 0;
    }

    void OsFileSync::Finalize() {
        if(file_.file_pointer != nullptr)
            fclose((FILE*)file_.file_pointer);
        if(file_.extra_file_info != nullptr)
            delete file_.extra_file_info;
        initialized_ = false;
    }

    std::size_t OsFileSync::Read(FileBuffer& buffer, std::size_t length) {
        if(mode_ == FileOperateMode::READ_BINARY) {
            buffer.size = length;
            buffer.buffer = new uint8_t[length];
            auto result = fread(buffer.buffer, buffer.size, 1, (FILE*)file_.file_pointer);
            if(result == 1)
                return length;
            else
                return 0;
        }
        else if(mode_ == FileOperateMode::READ_TEXT) {
            buffer.size = length + 1;
            buffer.buffer = new uint8_t[length + 1];
            auto result = fread(buffer.buffer, buffer.size, 1, (FILE*)file_.file_pointer);
            static_cast<char*>(buffer.buffer)[length] = '\0';
            if(result == 0)
                return buffer.size;
            else
                return 0;
        }
        else {
            return 0;
        }
    }

    std::size_t OsFileSync::ReadAll(FileBuffer& buffer) {
        if(mode_ == FileOperateMode::READ_BINARY || mode_ == FileOperateMode::READ_TEXT)
            return Read(buffer, file_.total_size);
        else 
            return 0;
    }

    std::size_t OsFileSync::Write(FileBuffer& buffer, std::size_t length) {
        std::size_t real_length = 0;
        if(length == 0 || length > buffer.size)
            real_length = buffer.size;
        else
            real_length = length;
        
        auto result = fwrite(buffer.buffer, real_length, 1, (FILE*)file_.file_pointer);
        if(result == 1)
            return length;
        else
            return 0;
    }

    std::size_t OsFileSync::WriteAll(FileBuffer& buffer) {
        auto result = fwrite(buffer.buffer, buffer.size, 1, (FILE*)file_.file_pointer);
        if(result == 1)
            return buffer.size;
        else
            return 0;
    }

    void OsFileSync::Seek(std::size_t position) {
        auto result = fseek((FILE*)file_.file_pointer, position, SEEK_SET);
        if(result != 0) {
            RK_ERROR(File, "{} File Seek {} Error", file_.full_name, position);
            throw std::runtime_error("File Seek Error");
        }
    }

    void OsFileSync::SeekToEnd(void) {
        auto result = fseek((FILE*)file_.file_pointer, 0, SEEK_END);
        if(result != 0) {
            RK_ERROR(File, "{} File Seek End Error", file_.full_name);
            throw std::runtime_error("File Seek End Error");
        }
    }

    void OsFileSync::Skip(std::size_t bytes) {
        auto result = fseek((FILE*)file_.file_pointer, bytes, SEEK_CUR);
        if(result != 0) {
            RK_ERROR(File, "{} File Skip {} Error", file_.full_name);
            throw std::runtime_error("File Skip Error");
        }
    }

    std::size_t OsFileSync::Tell(void) const {
        std::size_t result = ftell((FILE*)file_.file_pointer);
        return result;
    }
}
