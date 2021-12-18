#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"

#include <string>

namespace Rocket {
    class VirtualFileSystem {
        void Initialize();
        void Finalize();
        bool IsInitialized() const;
        // Mount and Unmount
        bool MountFileSystem(const FileSystemPtr& fs, const std::string& path);
        bool UnmountFileSystem(const FileSystemPtr& fs);
        bool UnmountFileSystem(const std::string& path);
        // VFS
        [[nodiscard]] VirtualBlockPtr RootBlock() const;
        [[nodiscard]] VNodeList VNodes(const std::string& dir) const;
        [[nodiscard]] VBlockList VBlocks(const std::string& dir) const;
        [[nodiscard]] const VNodeMap& VNodesMap() const;
        [[nodiscard]] const VBlockMap& VBlocksMap() const;
        // Basic Judgement
        [[nodiscard]] bool IsFileExists(const std::string& file_path) const;
        [[nodiscard]] bool IsDirExists(const std::string& dir_path) const;
        [[nodiscard]] bool IsFile(const std::string& file_path) const;
        [[nodiscard]] bool IsDir(const std::string& file_path) const;
        [[nodiscard]] bool IsReadOnly() const;
        // File Operation
        [[nodiscard]] FilePtr OpenFile(const std::string& file_path, int32_t mode);
        void CloseFile(const FilePtr& file);
        [[nodiscard]] std::size_t ReadFile(const FilePtr& file, FileBuffer* data);
        [[nodiscard]] std::size_t WriteFile(FilePtr& file, const FileBuffer& data);
        [[nodiscard]] bool CreateFile(const std::string& file_path);
        [[nodiscard]] bool RemoveFile(const std::string& file_path);
        [[nodiscard]] bool MoveFile(const std::string& src, const std::string& dst);
        [[nodiscard]] bool RenameFile(const std::string& src, const std::string& dst);
        [[nodiscard]] bool CopyFile(const std::string& src, const std::string& dst);
        [[nodiscard]] std::size_t FileSize(const FilePtr& file) const;
        // Dir Operation
        [[nodiscard]] bool CreateDir(const std::string& dir_path);
        [[nodiscard]] bool RemoveDir(const std::string& dir_path);
        [[nodiscard]] bool MoveDir(const std::string& src, const std::string& dst);
        [[nodiscard]] bool RenameDir(const std::string& src, const std::string& dst);
        [[nodiscard]] bool CopyDir(const std::string& src, const std::string& dst);
    };
}
