#include "FileSystem/FileSystem.h"
#include "Log/Log.h"

#include <exception>

namespace Rocket {
    std::unique_ptr<OsFile> FileSystem::OpenSync(
        const std::string& path, 
        const std::string& file_name, 
        FileOperateMode mode) {
        std::unique_ptr<OsFile> file(new OsFile);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: File Open Error", path, file_name);
            throw std::runtime_error("File Open Error");
        }
        return file;
    }

    void FileSystem::CloseSync(std::unique_ptr<OsFile>&& file) {
        file->Finalize();
    }

    std::unique_ptr<ZipFile> FileSystem::OpenZip(
        const std::string& path, 
        const std::string& file_name, 
        FileOperateMode mode) {
        std::unique_ptr<ZipFile> file(new ZipFile);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: Zip File Open Error", path, file_name);
            throw std::runtime_error("Zip File Open Error");
        }
        return file;
    }

    void FileSystem::CloseZip(std::unique_ptr<ZipFile>&& file) {
        file->Finalize();
    }

    std::unique_ptr<OsFileAsync> FileSystem::OpenAsync(
        const std::string& path, 
        const std::string& file_name, 
        FileOperateMode mode) {
        std::unique_ptr<OsFileAsync> file(new OsFileAsync);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: Async File Open Error", path, file_name);
            throw std::runtime_error("Async File Open Error");
        }
        return file;
    }

    void FileSystem::CloseAsync(std::unique_ptr<OsFileAsync>&& file) {
        file->Finalize();
    }
}