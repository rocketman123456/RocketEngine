#pragma once
#include "Core/MemoryDefine.h"
// #include "FileSystem/Basic/FileInfoCache.h"
// #include "FileSystem/Basic/FileBufferCache.h"

#include <string>
#include <set>

namespace Rocket {
    using FileList = std::set<FilePtr>;

    _Interface_ FileSystem {
    public:
        virtual ~FileSystem() = default;

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual bool IsInitialized() const = 0;
        virtual const std::string& BasePath() const = 0;
        virtual const FileList& Files() const = 0;
        virtual FilePtr OpenFile(const FileInfoPtr& file_path, int mode) = 0;
        virtual void CloseFile(const FilePtr& file) = 0;
        virtual bool CreateFile(const FileInfoPtr& file_path) = 0;
        virtual bool RemoveFile(const FileInfoPtr& file_path) = 0;
        virtual bool CopyFile(const FileInfoPtr& src, const FileInfoPtr& dest) = 0;
        virtual bool RenameFile(const FileInfoPtr& src, const FileInfoPtr& dest) = 0;
        virtual bool IsFileExists(const FileInfoPtr& file_path) const = 0;
        virtual bool IsFile(const FileInfoPtr& file_path) const = 0;
        virtual bool IsDir(const FileInfoPtr& file_path) const = 0;
        virtual bool IsReadOnly() const = 0;
    };

    CLASS_PTR(FileSystem);
}
