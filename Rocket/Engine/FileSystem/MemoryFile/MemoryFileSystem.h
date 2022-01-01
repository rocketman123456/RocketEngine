#pragma once
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/MemoryFile/MemoryFile.h"

#include <unordered_map>

namespace Rocket {
    class MemoryFileSystem : _implements_ FileSystem {
        using MemoryFileMap = std::unordered_map<std::string, FileBufferPtr>;
    public:
        MemoryFileSystem(const std::string& real_path);
        MemoryFileSystem(const std::string& real_path, const std::string& virtual_path);
        virtual ~MemoryFileSystem() = default;
        // Basic Operation
        void Initialize() final;
        void Finalize() final;
        // For File System
        void SetVirtualPath(const std::string& basic) final;
        void SetRealPath(const std::string& alias) final;
        // Basic Judgement
        bool IsReadOnly() const final { return false; }
        // File Operation
        FilePtr GetFilePointer(const std::string& file_path) final;
        bool CreateFile(const std::string& file_path) final;
        bool RemoveFile(const std::string& file_path) final;
        // virtual bool MoveFile(const std::string& src, const std::string& dst);
        // virtual bool RenameFile(const std::string& src, const std::string& dst);
        // virtual bool CopyFile(const std::string& src, const std::string& dst);
        // virtual std::size_t FileSize(const FilePtr& file) const;
        // Dir Operation
        bool CreateDir(const std::string& dir_path) final;
        bool RemoveDir(const std::string& dir_path) final;
        // virtual bool MoveDir(const std::string& src, const std::string& dst);
        // virtual bool RenameDir(const std::string& src, const std::string& dst);
        // virtual bool CopyDir(const std::string& src, const std::string& dst);
    private:
        void NormalizePath();
        void GetRootName();
        void RemoveFileRecursive(VirtualBlockPtr& block);
    private:
        MemoryFileMap file_map;
    };

    CLASS_PTR(MemoryFileSystem);
}
