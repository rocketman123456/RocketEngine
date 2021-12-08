#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/FileSystem.h"

#include <unordered_map>
#include <string>
#include <list>

namespace Rocket {
    class VirtualFileSystem final {
    public:
        using FileSystemList = std::list<FileSystemPtr>;
        using FileSystemMap = std::unordered_map<std::string, FileSystemPtr>;
        using FileMap = std::unordered_map<std::uintptr_t, FileSystemPtr>;
        struct SortedAlias {
            std::string alias;
            FileSystemPtr filesystem;
            
            SortedAlias(const std::string& a, FileSystemPtr fs) : alias(a), filesystem(fs) {}
        };
        using SortedAliasList = std::list<SortedAlias>;
    public:
        VirtualFileSystem();
        ~VirtualFileSystem();

        void AddFileSystem(const std::string& alias, FileSystemPtr filesystem);
        void RemoveFileSystem(const std::string& alias);
        bool IsFileSystemExists(const std::string& alias) const;
        FileSystemPtr GetFilesystem(const std::string& alias);
        FilePtr OpenFile(const FileInfoPtr& filePath, FileMode::FileOperate mode);
        void CloseFile(FilePtr file);
    private:
        FileSystemMap file_system;
        SortedAliasList sorted_alias;
        FileMap opened_files;
    };

    CLASS_PTR(VirtualFileSystem);

    extern void vfs_initialize();
    extern void vfs_shutdown();
    extern VirtualFileSystemPtr vfs_get_global();
}
