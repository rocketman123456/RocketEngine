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
        // Dir Operation

    private:
        void CreateVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);

    private:
        MemoryFileMap file_map;
    };

    CLASS_PTR(MemoryFileSystem);
}
