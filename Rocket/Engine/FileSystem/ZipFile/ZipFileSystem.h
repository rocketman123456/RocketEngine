#pragma once
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/ZipFile/ZipFile.h"

#include <zip.h>

namespace Rocket {
    class ZipFileSystem : _implements_ FileSystem {
    public:
        ZipFileSystem(const std::string& real_path);
        ZipFileSystem(const std::string& real_path, const std::string& virtual_path);
        virtual ~ZipFileSystem() = default;
        // Basic Operation
        void Initialize() final;
        void Finalize() final;
        // For File System
        void SetVirtualPath(const std::string& basic) final;
        void SetRealPath(const std::string& alias) final;
        // Basic Judgement
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
        void BuildVirtualSystem();
        VirtualBlockPtr CreateVirtualBlock(VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
        VirtualNodePtr CreateVirtualNode(VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    private:
        zip_t* zip_archive = nullptr;
    };

    CLASS_PTR(ZipFileSystem);
}
