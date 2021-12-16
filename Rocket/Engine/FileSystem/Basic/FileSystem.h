#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"
#include "Utils/CRC.h"

#include <vector>
#include <gsl/gsl>

namespace Rocket {
    using VNodeList = std::vector<VirtualNodePtr>;
    using VBlockList = std::vector<VirtualBlockPtr>;

    _Interface_ FileSystem {
    public:
        virtual ~FileSystem() = default;
        // Basic Operation
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
        virtual VBlockList VBlocks(const std::string& dir) const = 0;
        // Basic Judgement
        virtual bool IsFileExists(const std::string& file_path) const = 0;
        virtual bool IsDirExists(const std::string& dir_path) const = 0;
        virtual bool IsFile(const std::string& file_path) const = 0;
        virtual bool IsDir(const std::string& file_path) const = 0;
        virtual bool IsReadOnly() const = 0;
        // File Operation
        virtual FilePtr OpenFile(const std::string& file_path, int32_t mode) = 0;
        virtual void CloseFile(const FilePtr& file) = 0;
        virtual std::size_t ReadFile(const FilePtr& file, FileBuffer& data) = 0;
        virtual std::size_t WriteFile(FilePtr& file, const FileBuffer& data) = 0;
        virtual bool CreateFile(const std::string& file_path) = 0;
        virtual bool RemoveFile(const std::string& file_path) = 0;
        // virtual bool MoveFile(const std::string& src, const std::string& dst) = 0;
        // virtual bool RenameFile(const std::string& src, const std::string& dst) = 0;
        // virtual bool CopyFile(const std::string& src, const std::string& dst) = 0;
        virtual std::size_t FileSize(const FilePtr& file) const = 0;
        // Dir Operation
        virtual bool CreateDir(const std::string& dir_path) = 0;
        virtual bool RemoveDir(const std::string& dir_path) = 0;
        // virtual bool MoveDir(const std::string& src, const std::string& dst) = 0;
        // virtual bool RenameDir(const std::string& src, const std::string& dst) = 0;
        // virtual bool CopyDir(const std::string& src, const std::string& dst) = 0;
    };

    CLASS_PTR(FileSystem);
}
