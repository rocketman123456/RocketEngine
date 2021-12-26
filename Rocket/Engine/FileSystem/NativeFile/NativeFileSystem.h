#pragma once
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/NativeFile/NativeFile.h"

#include <filesystem>

namespace Rocket {
    class NativeFileSystem : _implements_ FileSystem {
    public:
        NativeFileSystem(const std::string& real_path);
        NativeFileSystem(const std::string& real_path, const std::string& virtual_path);
        virtual ~NativeFileSystem() = default;
        // Basic Operation
        void Initialize() final;
        void Finalize() final;
        // For File System
        void SetVirtualPath(const std::string& vpath) final;    // Will Update File System
        void SetRealPath(const std::string& rpath) final;       // Will Update File System
        // Basic Judgement in Virtual Path
        bool IsReadOnly() const final;
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
        void CheckFileSystem();
        void GetRootName();
        void BuildVirtualSystem(const std::filesystem::path& path, VirtualBlockPtr& root);
        void BuildVirtualSystem();
    };

    CLASS_PTR(NativeFileSystem);
}
