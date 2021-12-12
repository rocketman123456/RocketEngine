#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/Basic/VirtualBlock.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "Utils/CRC.h"

#include <unordered_map>

namespace Rocket  {
    class VirtualFileSystem {
    public:
        using VirtualBlockMap = std::unordered_map<std::string, VirtualBlockPtr>;
    public:
        VirtualFileSystem() = default;
        ~VirtualFileSystem() = default;

        void MountFileSystem(const std::string& alias, FileSystemPtr filesystem);
        void UnmountFileSystem(const std::string& alias);
        FileSystemPtr GetFilesystem(const std::string& dir);
        // file operation
        FilePtr OpenFile(const std::string& filePath, FileMode::FileOperate mode);
        void CloseFile(FilePtr file);
        bool CreateFile(const std::string& file_path);
        bool RemoveFile(const std::string& file_path);
        bool CreateDir(const std::string& file_path);
        bool RemoveDir(const std::string& file_path);
        bool CreateLink(const std::string& src, const std::string& dst);
        bool RemoveLink(const std::string& target);
        bool CopyFile(const std::string& src, const std::string& dest);
        bool RenameFile(const std::string& src, const std::string& dest);
        // checker
        bool IsFileExists(const std::string& file_path) const;
        bool IsDirectoryExists(const std::string& file_path) const;
        bool IsFile(const std::string& file_path) const;
        bool IsDirectory(const std::string& file_path) const;
        bool IsReadOnly() const;
    private:
        void SetupFileSystemRecurisively(VirtualBlockPtr block, const FileSystemPtr& file_system);
    private:
        VirtualBlockPtr root_block = nullptr;
        VirtualBlockMap directory_map = {};
        CRC32 crc32;
    };

    CLASS_PTR(VirtualFileSystem);
}
