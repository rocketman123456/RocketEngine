#pragma once
#include "FileSystem/NativeFile/NativeFile.h"
#include "FileSystem/Basic/FileSystem.h"

namespace Rocket {
    const uint64_t CHUNK_SIZE = 1024;
    struct SDir;
    
    class NativeFileSystem : _implements_ FileSystem {
    public:
        NativeFileSystem(const std::string& base);
        virtual ~NativeFileSystem() = default;

        inline bool IsInitialized() const final { return is_initialized; }
        inline const std::string& BasePath() const final { return base_path; }
        inline const FileList& Files() const final { return file_list; }

        void Initialize() final;
        void Finalize() final;
        FilePtr OpenFile(const FileInfoPtr& file_path, int mode) final;
        void CloseFile(const FilePtr& file) final;
        bool CreateFile(const FileInfoPtr& file_path) final;
        bool RemoveFile(const FileInfoPtr& file_path) final;
        bool CopyFile(const FileInfoPtr& src, const FileInfoPtr& dest) final;
        bool RenameFile(const FileInfoPtr& src, const FileInfoPtr& dest) final;
        bool IsFileExists(const FileInfoPtr& file_path) const final;
        bool IsFile(const FileInfoPtr& file_path) const final;
        bool IsDir(const FileInfoPtr& dir_path) const final;
        bool IsReadOnly() const final;
    private:
        FilePtr FindFile(const FileInfoPtr& fileInfo) const;
        void BuildFilelist(SDir* dir, const std::string& basePath, FileList& out);
    private:
        std::string base_path;
        FileList file_list;
        bool is_initialized = false;
    };

    CLASS_PTR(NativeFileSystem);
}
