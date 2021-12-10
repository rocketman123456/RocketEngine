#pragma once
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/NativeFile/NativeFile.h"

namespace Rocket {
    class NativeFileSystem : _implements_ FileSystem {
    public:
        virtual ~NativeFileSystem() = default;
        // Prepare and Destroy
        void Initialize() final;
        void Finalize() final;
        bool IsInitialized() const final;
        // For File System
        const std::string& BasePath() const final;
        const FileList& Files(const std::string& dir) const final;
        FilePtr OpenFile(const std::string& file_path, int32_t mode) final;
        void CloseFile(const FilePtr& file) final;
        bool CreateFile(const std::string& file_path) final;
        bool RemoveFile(const std::string& file_path) final;
        bool IsFileExists(const std::string& file_path) const final;
        bool IsFile(const std::string& file_path) const final;
        bool IsDir(const std::string& file_path) const final;
        bool IsReadOnly() const final;
        // For VFS
        VirtualBlockPtr BuildUpVirtualBlocks() final;
    private:
        std::string native_path = "";
        std::string alias_path = "";
    };
}
