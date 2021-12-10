#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/VirtualBlock.h"

#include <set>

namespace Rocket {
    using FileList = std::set<FilePtr>;
    class VirtualFileSystem;
    // Handle File in Different Types
    _Interface_ FileSystem {
        friend class VirtualFileSystem;
    public:
        virtual ~FileSystem() = default;

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual bool IsInitialized() const = 0;
        // For File System
        virtual const std::string& BasePath() const = 0;
        virtual const FileList& Files(const std::string& dir) const = 0;
        virtual FilePtr OpenFile(const std::string& file_path, int32_t mode) = 0;
        virtual void CloseFile(const FilePtr& file) = 0;
        virtual bool CreateFile(const std::string& file_path) = 0;
        virtual bool RemoveFile(const std::string& file_path) = 0;
        virtual bool IsFileExists(const std::string& file_path) const = 0;
        virtual bool IsFile(const std::string& file_path) const = 0;
        virtual bool IsDir(const std::string& file_path) const = 0;
        virtual bool IsReadOnly() const = 0;
    protected:
        virtual VirtualBlockPtr BuildUpVirtualBlocks() = 0;
    };

    CLASS_PTR(FileSystem);
}
