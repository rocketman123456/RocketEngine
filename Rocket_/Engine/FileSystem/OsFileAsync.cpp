#include "FileSystem/OsFileAsync.h"

namespace Rocket {
    int32_t OsFileAsync::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        return OsFileSync::Initialize(path, file_name, mode);
    }

    int32_t OsFileAsync::Initialize(const std::string& path, FileOperateMode mode) {
        return OsFileSync::Initialize(path, mode);
    }
    
    void OsFileAsync::Finalize() {
        OsFileSync::Finalize();
    }

    std::future<std::size_t> OsFileAsync::ReadAsync(FileBuffer& buffer, std::size_t length) {
        return std::async(std::launch::async, [&](){ return this->Read(buffer, length); });
    }

    std::future<std::size_t> OsFileAsync::ReadAllAsync(FileBuffer& buffer) {
        return std::async(std::launch::async, [&](){ return this->ReadAll(buffer); });
    }

    std::future<std::size_t> OsFileAsync::WriteAsync(FileBuffer& buffer, std::size_t length) {
        return std::async(std::launch::async, [&](){ return this->Write(buffer, length); });
    }

    std::future<std::size_t> OsFileAsync::WriteAllAsync(FileBuffer& buffer) {
        return std::async(std::launch::async, [&](){ return this->WriteAll(buffer); });
    }
}