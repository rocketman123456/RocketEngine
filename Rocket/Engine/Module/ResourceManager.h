#pragma once
#include "Interface/IRuntimeModule.h"
#include "FileSystem/OsFile.h"
#include "FileSystem/OsFileOperation.h"

#include <string>
#include <cstdint>
#include <future>

namespace Rocket {
    enum class ResourceType : uint32_t {
        kCompress = 0,
        kNormal,
    };

    class ResourceManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(ResourceManager);
    public:
        virtual ~ResourceManager() = default;
        
        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;

        std::unique_ptr<FileSystem::OsSyncFile> OpenSync(const char* file_name, FileSystem::FileMode mode);
        void CloseSync(FileSystem::OsSyncFile&& file);

        ///// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        //size_t ReadSync(FileSystem::OsFileBuffer* buffer, size_t length);
        ///// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
        //size_t WriteSync(const FileSystem::OsFileBuffer* buffer, size_t length);

        ///// Seeks to the desired position
        //void Seek(size_t position);
        ///// Seeks to the end of the file
        //void SeekToEnd(void);
        ///// Skips a certain amount of bytes
        //void Skip(size_t bytes);
        ///// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
        //size_t Tell(void) const;

        std::unique_ptr<FileSystem::OsAsyncFile> OpenAsync(const char* file_name, FileSystem::FileMode mode);
        void CloseAsync(FileSystem::OsAsyncFile&& file);

        ///// Asynchronously reads from the file into a buffer
        //FileSystem::OsFileAsyncOperation ReadAsync(FileSystem::OsFileBuffer* buffer, size_t length);
        ///// Asynchronously writes from a buffer into the file
        //FileSystem::OsFileAsyncOperation WriteAsync(const FileSystem::OsFileBuffer* buffer, size_t length);
        ///// Waits until the asynchronous operation has finished
        //void WaitForAsync(FileSystem::OsFileAsyncOperation operation);
        ///// Returns the number of bytes transferred in the asynchronous operation
        //size_t GetAsyncBytesTransferred(FileSystem::OsFileAsyncOperation operation) const;

    private:
        std::string root_path_;
        std::string asset_path_;
        ResourceType resource_type_;
    };

    extern IRuntimeModule* g_ResourceManager;
}