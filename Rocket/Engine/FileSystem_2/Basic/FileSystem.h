#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/VirtualBlock.h"
#include "Utils/CRC.h"

#include <set>

namespace Rocket {
    using VNodeList = std::set<VirtualNodePtr>;
    // Handle File in Different Types
    _Interface_ FileSystem {
    public:
        virtual ~FileSystem() = default;

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual bool IsInitialized() const = 0;
        // For File System
        virtual void SetBasicPath(const std::string& basic) = 0;
        virtual void SetAliasPath(const std::string& alias) = 0;
        virtual const std::string& BasicPath() const = 0;
        virtual const std::string& AliasPath() const = 0;
        virtual VirtualBlockPtr RootBlock() const = 0;
        virtual VNodeList VNodes(const std::string& dir) const = 0;
        // File Operation
        virtual FilePtr OpenFile(const std::string& file_path, int32_t mode) = 0;
        virtual void CloseFile(const FilePtr& file) = 0;
        virtual bool CreateFile(const std::string& file_path) = 0;
        virtual bool RemoveFile(const std::string& file_path) = 0;
        virtual bool CreateDir(const std::string& dir_path) = 0;
        virtual bool RemoveDir(const std::string& dir_path) = 0;
        virtual bool IsFileExists(const std::string& file_path) const = 0;
        virtual bool IsDirExists(const std::string& dir_path) const = 0;
        virtual bool IsFile(const std::string& file_path) const = 0;
        virtual bool IsDir(const std::string& file_path) const = 0;
        virtual bool IsReadOnly() const = 0;
    };

    CLASS_PTR(FileSystem);
}