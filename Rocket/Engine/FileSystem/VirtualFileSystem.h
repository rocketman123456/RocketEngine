#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"
#include "Pattern/Singleton.h"

#include <string>
#include <gsl/gsl>

namespace Rocket {
    class VirtualFileSystem { // : _implements_ AbstractSingleton<VirtualFileSystem> {
    public:
        //using FileSystemMap = std::unordered_map<std::string, FileSystemPtr>;
        using FileSystemMap = std::vector<std::pair<std::string, FileSystemPtr>>;
    public:
        // Mount and Unmount
        [[nodiscard]] bool MountFileSystem(const FileSystemPtr& fs, const std::string& path);
        [[nodiscard]] bool UnmountFileSystem(const FileSystemPtr& fs);
        [[nodiscard]] bool UnmountFileSystem(const std::string& path);
        // VFS
        [[nodiscard]] inline VirtualBlockPtr RootBlock() const { return root; }
        // [[nodiscard]] inline const VNodeMap& NodeMap() const { return node_map; }
        // [[nodiscard]] inline const VBlockMap& BlockMap() const { return block_map; }
        [[nodiscard]] VNodeList VNodes(const std::string& dir) const;
        [[nodiscard]] VBlockList VBlocks(const std::string& dir) const;
        // Basic Judgement
        [[nodiscard]] bool IsFileExists(const std::string& file_path) const;
        [[nodiscard]] bool IsDirExists(const std::string& dir_path) const;
        [[nodiscard]] bool IsFile(const std::string& file_path) const;
        [[nodiscard]] bool IsDir(const std::string& file_path) const;
        // [[nodiscard]] bool IsFileReadOnly(const std::string& file_path) const;
        // [[nodiscard]] bool IsDirReadOnly(const std::string& file_path) const;
        // File Operation
        [[nodiscard]] FilePtr GetFilePointer(const std::string& file_path);
        void OpenFile(const FilePtr& file, int32_t mode);
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

    private:
        void SetupBlockFileSystem(VirtualBlockPtr& root, const FileSystemPtr& fs);

    private:
        VirtualBlockPtr root = nullptr;
        FileSystemMap mounted_file_system;
    };

    CLASS_PTR(VirtualFileSystem);
}
