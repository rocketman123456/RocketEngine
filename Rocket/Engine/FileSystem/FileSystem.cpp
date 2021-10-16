#include "FileSystem/FileSystem.h"
#include "Log/Log.h"

#include <exception>

namespace Rocket {
    void FileSystem::LoadSync(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer) {
        auto file = FileSystem::OpenSync(path, file_name, mode);
        file->ReadAll(buffer);
        FileSystem::CloseSync(std::move(file));
    }
    
    // void FileSystem::LoadAsync(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer) {
    // 
    // }
    
    void FileSystem::LoadZip(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer) {
        auto file = FileSystem::OpenZip(path, file_name, mode);
        // TODO 
        FileSystem::CloseZip(std::move(file));
    }
    
    void FileSystem::LoadAudio(const std::string& path, const std::string& file_name, FileOperateMode mode, FileBuffer& buffer) {
        auto file = FileSystem::OpenAudio(path, file_name, mode);
        file->ReadAll(buffer);
        FileSystem::CloseAudio(std::move(file));
    }

    void FileSystem::LoadSync(const std::string& path, FileOperateMode mode, FileBuffer& buffer) {
        auto file = FileSystem::OpenSync(path, mode);
        file->ReadAll(buffer);
        FileSystem::CloseSync(std::move(file));
    }
    
    // void FileSystem::LoadAsync(const std::string& path, FileOperateMode mode) {
    //
    // }
    
    void FileSystem::LoadZip(const std::string& path, FileOperateMode mode, FileBuffer& buffer) {
        auto file = FileSystem::OpenZip(path, mode);
        // TODO
        FileSystem::CloseZip(std::move(file));
    }
    
    void FileSystem::LoadAudio(const std::string& path, FileOperateMode mode, FileBuffer& buffer) {
        auto file = FileSystem::OpenAudio(path, mode);
        file->ReadAll(buffer);
        FileSystem::CloseAudio(std::move(file));
    }

    OsFileSyncPtr FileSystem::OpenSync(const std::string& path, FileOperateMode mode) {
        OsFileSyncPtr file(new OsFileSync);
        int32_t result = file->Initialize(path, mode);
        if(result != 0) {
            RK_ERROR(Core, "{}: File Open Error", path);
            throw std::runtime_error("File Open Error");
        }
        return file;
    }

    OsFileAsyncPtr FileSystem::OpenAsync(const std::string& path, FileOperateMode mode) {
        OsFileAsyncPtr file(new OsFileAsync);
        int32_t result = file->Initialize(path, mode);
        if(result != 0) {
            RK_ERROR(Core, "{}: Async File Open Error", path);
            throw std::runtime_error("Async File Open Error");
        }
        return file;
    }

    ZipFilePtr FileSystem::OpenZip(const std::string& path, FileOperateMode mode) {
        ZipFilePtr file(new ZipFile);
        int32_t result = file->Initialize(path, mode);
        if(result != 0) {
            RK_ERROR(Core, "{}: Zip File Open Error", path);
            throw std::runtime_error("Zip File Open Error");
        }
        return file;
    }

    AudioFilePtr FileSystem::OpenAudio(const std::string& path, FileOperateMode mode) {
        AudioFilePtr file(new AudioFile);
        int32_t result = file->Initialize(path, mode);
        if(result != 0) {
            RK_ERROR(Core, "{}: Audio File Open Error", path);
            throw std::runtime_error("Audio File Open Error");
        }
        return file;
    }

    OsFileSyncPtr FileSystem::OpenSync(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        OsFileSyncPtr file(new OsFileSync);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: File Open Error", path, file_name);
            throw std::runtime_error("File Open Error");
        }
        return file;
    }

    OsFileAsyncPtr FileSystem::OpenAsync(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        std::unique_ptr<OsFileAsync> file(new OsFileAsync);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: Async File Open Error", path, file_name);
            throw std::runtime_error("Async File Open Error");
        }
        return file;
    }

    ZipFilePtr FileSystem::OpenZip(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        ZipFilePtr file(new ZipFile);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: Zip File Open Error", path, file_name);
            throw std::runtime_error("Zip File Open Error");
        }
        return file;
    }

    AudioFilePtr FileSystem::OpenAudio(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        AudioFilePtr file(new AudioFile);
        int32_t result = file->Initialize(path, file_name, mode);
        if(result != 0) {
            RK_ERROR(Core, "{},{}: Audio File Open Error", path, file_name);
            throw std::runtime_error("Audio File Open Error");
        }
        return file;
    }

    void FileSystem::CloseZip(ZipFilePtr&& file) {
        file->Finalize();
        file.reset();
    }

    void FileSystem::CloseAsync(OsFileAsyncPtr&& file) {
        file->Finalize();
        file.reset();
    }

    void FileSystem::CloseSync(OsFileSyncPtr&& file) {
        file->Finalize();
        file.reset();
    }

    void FileSystem::CloseAudio(AudioFilePtr&& file) {
        file->Finalize();
        file.reset();
    }
}
