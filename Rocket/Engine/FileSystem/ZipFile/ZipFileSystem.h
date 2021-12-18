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
        inline bool IsInitialized() const final { return is_initialized; }
        // For File System
        void SetVirtualPath(const std::string& basic) final;
        void SetRealPath(const std::string& alias) final;
        inline const std::string& VirtualPath() const final { return virtual_path; }
        inline const std::string& RealPath() const final { return real_path; }
        inline VirtualBlockPtr RootBlock() const final { return root; }
        VNodeList VNodes(const std::string& dir) const final;
        VBlockList VBlocks(const std::string& dir) const final;
        inline const VNodeMap& VNodesMap() const final { return node_map; }
        inline const VBlockMap& VBlocksMap() const final { return block_map; }
        // Basic Judgement
        bool IsFileExists(const std::string& file_path) const final;
        bool IsDirExists(const std::string& dir_path) const final;
        bool IsFile(const std::string& file_path) const final;
        bool IsDir(const std::string& dir_path) const final;
        bool IsReadOnly() const final;
        // File Operation
        FilePtr OpenFile(const std::string& file_path, int32_t mode) final;
        void CloseFile(const FilePtr& file) final;
        std::size_t ReadFile(const FilePtr& file, FileBuffer* data) final;
        std::size_t WriteFile(FilePtr& file, const FileBuffer& data) final;
        bool CreateFile(const std::string& file_path) final;
        bool RemoveFile(const std::string& file_path) final;
        std::size_t FileSize(const FilePtr& file) const final;
        // Dir Operation
        bool CreateDir(const std::string& dir_path) final;
        bool RemoveDir(const std::string& dir_path) final;

    private:
        void NormalizePath();
        void CheckFileSystem();
        void GetRootName();
        void BuildVirtualSystem();

    private:
        VirtualBlockPtr root = nullptr;
        std::string real_path = "";
        std::string virtual_path = "";
        VNodeMap node_map = {};
        VBlockMap block_map = {};
        zip_t* zip_archive = nullptr;
        bool is_initialized = false;
    };

    CLASS_PTR(ZipFileSystem);
}
