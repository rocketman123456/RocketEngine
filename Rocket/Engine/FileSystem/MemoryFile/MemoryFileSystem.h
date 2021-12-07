#pragma once
#include "FileSystem/MemoryFile/MemoryFile.h"
#include "FileSystem/Basic/FileSystem.h"

namespace Rocket {
    class MemoryFileSystem : _implements_ FileSystem {
    public:
        MemoryFileSystem() = default;
        virtual ~MemoryFileSystem() = default;

        inline bool IsInitialized() const final { return is_initialized; }
        inline const std::string& BasePath() const final { static std::string base_path = "/"; return base_path; }
        inline const FileList& Files() const final { return file_list; }
        inline bool IsReadOnly() const final { return false; }

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
    private:
        FilePtr FindFile(const FileInfoPtr& fileInfo) const;
    private:
        FileList file_list;
        bool is_initialized = false;
    };

    CLASS_PTR(MemoryFileSystem);
}