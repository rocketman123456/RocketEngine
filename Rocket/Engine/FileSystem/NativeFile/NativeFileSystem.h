#pragma once
#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/NativeFile/NativeFile.h"

#include <filesystem>
#include <string>

namespace Rocket {
    class NativeFileSystem : _implements_ FileSystem {
    public:
        NativeFileSystem(const std::string& basic_);
        NativeFileSystem(const std::string& basic_, const std::string& alias_);
        virtual ~NativeFileSystem() = default;

        inline void SetBasicPath(const std::string& basic) final { basic_path = basic; }
        inline void SetAliasPath(const std::string& alias) final { alias_path = alias; }
        inline const std::string& BasicPath() const final { return basic_path; }
        inline const std::string& AliasPath() const final { return alias_path; }
        inline VirtualBlockPtr RootBlock() const final { return root; }
        inline bool IsInitialized() const final { return is_initialized; }

        // Prepare and Destroy
        void Initialize() final;
        void Finalize() final;
        // For File System
        const FileList& Files(const std::string& dir) const final;
        FilePtr OpenFile(const std::string& file_path, int32_t mode) final;
        void CloseFile(const FilePtr& file) final;
        bool CreateFile(const std::string& file_path) final;
        bool RemoveFile(const std::string& file_path) final;
        bool CreateDir(const std::string& dir_path) final;
        bool RemoveDir(const std::string& dir_path) final;
        bool IsFileExists(const std::string& file_path) const final;
        bool IsFile(const std::string& file_path) const final;
        bool IsDir(const std::string& file_path) const final;
        bool IsReadOnly() const final;
    private:
        // For VFS
        void BuildUpVirtualBlocks(const std::filesystem::path& path, VirtualBlockPtr& block);
    private:
        std::string basic_path = "";
        std::string alias_path = "";
        VirtualBlockPtr root = nullptr;
        bool is_initialized = false;
    };

    CLASS_PTR(NativeFileSystem);
}
