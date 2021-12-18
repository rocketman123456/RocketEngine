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
        // Dir Operation

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
