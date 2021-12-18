#pragma once
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/MemoryFile/MemoryFile.h"

#include <unordered_map>

namespace Rocket {
    class MemoryFileSystem : _implements_ FileSystem {
        using MemoryFileMap = std::unordered_map<std::string, MemoryFilePtr>;
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
        // virtual FilePtr GetFilePointer(const std::string& file_path) = 0;
        // virtual bool CreateFile(const std::string& file_path);
        // virtual bool RemoveFile(const std::string& file_path);
        // virtual bool MoveFile(const std::string& src, const std::string& dst);
        // virtual bool RenameFile(const std::string& src, const std::string& dst);
        // virtual bool CopyFile(const std::string& src, const std::string& dst);
        // virtual std::size_t FileSize(const FilePtr& file) const;
        // Dir Operation
        // virtual bool CreateDir(const std::string& dir_path);
        // virtual bool RemoveDir(const std::string& dir_path);
        // virtual bool MoveDir(const std::string& src, const std::string& dst);
        // virtual bool RenameDir(const std::string& src, const std::string& dst);
        // virtual bool CopyDir(const std::string& src, const std::string& dst);

    private:
        void CreateVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);

    private:
        MemoryFileMap file_map;
    };

    CLASS_PTR(MemoryFileSystem);
}
