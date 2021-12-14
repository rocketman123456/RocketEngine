#include "FileSystem/MemoryFile/MemoryFileSystem.h"

namespace Rocket {
    void MemoryFileSystem::Initialize() {
        if(is_initialized) {
            return;
        }
        is_initialized = true;
    }

    void MemoryFileSystem::Finalize() {
        file_list.clear();
        is_initialized = false;
    }

    FilePtr MemoryFileSystem::OpenFile(const FileInfoPtr& file_path, int mode) {
        FileInfoPtr fileInfo = std::make_shared<FileInfo>(BasePath(), file_path->AbsolutePath(), false);
        FilePtr file = FindFile(fileInfo);
        bool isExists = (file != nullptr);
        if (!isExists) {
            file.reset(new MemoryFile(fileInfo));
        }
        file->Open(mode);
        if (!isExists && file->IsOpened()) {
            file_list.insert(file);
        }
        return file;
    }

    void MemoryFileSystem::CloseFile(const FilePtr& file) {
        if (file) {
            file->Close();
        }
    }

    bool MemoryFileSystem::CreateFile(const FileInfoPtr& file_path) {
        bool result = false;
        if (!IsReadOnly() && !IsFileExists(file_path)) {
            FileInfoPtr fileInfo = std::make_shared<FileInfo>(BasePath(), file_path->AbsolutePath(), false);
            FilePtr file = OpenFile(file_path, FileMode::WRITE_BINARY | FileMode::TRUNCATE);
            if (file) {
                result = true;
                file->Close();
            }
        } else {
            result = true;
        }
        return result;
    }

    bool MemoryFileSystem::RemoveFile(const FileInfoPtr& file_path) {
        bool result = true;
        FilePtr file = FindFile(file_path);
        if (!IsReadOnly() && file) {
            FileInfoPtr fileInfo = std::make_shared<FileInfo>(BasePath(), file->FileInfo()->AbsolutePath(), false);
            file_list.erase(file);
        }
        return result;
    }

    bool MemoryFileSystem::CopyFile(const FileInfoPtr& src, const FileInfoPtr& dest) {
        bool result = false;
        if (!IsReadOnly()) {
            MemoryFilePtr srcFile = std::static_pointer_cast<MemoryFile>(FindFile(src));
            MemoryFilePtr dstFile = std::static_pointer_cast<MemoryFile>(OpenFile(dest, FileMode::WRITE_BINARY));
            if (srcFile && dstFile) {
                dstFile->file_data = {new gsl::byte[srcFile->file_data.size()], srcFile->file_data.size()};
                std::memcpy(dstFile->file_data.data(), srcFile->file_data.data(), srcFile->file_data.size());
                dstFile->Close();
                result = true;
            }
        }
        return result;
    }

    bool MemoryFileSystem::RenameFile(const FileInfoPtr& src, const FileInfoPtr& dest) {
        bool result = CopyFile(src, dest);
        if (result) {
            result = RemoveFile(src);
        }
        return result;
    }

    bool MemoryFileSystem::IsFileExists(const FileInfoPtr& file_path) const {
        auto info = std::make_shared<FileInfo>(BasePath() + file_path->AbsolutePath());
        return (FindFile(info) != nullptr);
    }

    bool MemoryFileSystem::IsFile(const FileInfoPtr& file_path) const {
        FilePtr file = FindFile(file_path);
        if (file) {
            return !file->FileInfo()->IsDir();
        }
        return false;
    }

    bool MemoryFileSystem::IsDir(const FileInfoPtr& dir_path) const {
        FilePtr file = FindFile(dir_path);
        if (file) {
            return file->FileInfo()->IsDir();
        }
        return false;
    }

    FilePtr MemoryFileSystem::FindFile(const FileInfoPtr& fileInfo) const {
        FileList::const_iterator it = std::find_if(file_list.begin(), file_list.end(), [&](FilePtr file) {
            return file->FileInfo() == fileInfo;
        });
        if (it != file_list.end()) {
            return *it;
        }
        return nullptr;
    }
}
