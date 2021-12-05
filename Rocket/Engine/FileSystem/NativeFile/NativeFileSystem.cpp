#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <sys/stat.h>
#include <dirent.h>
#include <vector>

namespace Rocket {
    struct SDir : public DIR {};

    NativeFileSystem::NativeFileSystem(const std::string& base) : base_path(base) {
        if(!EndsWith(base_path, "/")) {
            base_path += "/";
        }
    }

    void NativeFileSystem::Initialize() {
        if(is_initialized) {
            RK_INFO(File, "Double Initialize Native File System");
            return;
        }

        SDir* dir = static_cast<SDir*>(opendir(BasePath().c_str()));
        if (dir != nullptr) {
            BuildFilelist(dir, BasePath(), file_list);
            is_initialized = true;
            closedir(dir);
        }
    }

    void NativeFileSystem::Finalize() {
        base_path = "";
        file_list.clear();
        is_initialized = false;
    }

    FilePtr NativeFileSystem::OpenFile(const FileInfoPtr& file_path, int mode) {
        FileInfoPtr file_info = make_shared<FileInfo>(BasePath(), file_path->AbsolutePath(), false);
        FilePtr file = FindFile(file_info);
        bool is_exists = (file != nullptr);
        if (!is_exists) {
            mode |= FileMode::TRUNCATE;
            file.reset(new NativeFile(file_info));
        }
        file->Open(mode);
        
        if (!is_exists && file->IsOpened()) {
            file_list.insert(file);
        }
        
        return file;
    }

    void NativeFileSystem::CloseFile(const FilePtr& file) {
        if(file) {
            file->Close();
            //file_list.erase(file);
        }
    }

    bool NativeFileSystem::CreateFile(const FileInfoPtr& file_path) {
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

    bool NativeFileSystem::RemoveFile(const FileInfoPtr& file_path) {
        bool result = true;
        FilePtr file = FindFile(file_path);
        if (!IsReadOnly() && file) {
            FileInfoPtr file_info = std::make_shared<FileInfo>(BasePath(), file->FileInfo()->AbsolutePath(), false);
            if (remove(file_info->AbsolutePath().c_str())) {
                file_list.erase(file);
            }
        }
        return result;
    }

    bool NativeFileSystem::CopyFile(const FileInfoPtr& src, const FileInfoPtr& dest) {
        bool result = false;
        if (!IsReadOnly()) {
            FilePtr fromFile = FindFile(src);
            FilePtr toFile = OpenFile(dest, FileMode::WRITE_BINARY);
            
            if (fromFile && toFile) {
                uint64_t size = kChunkSize;
                std::vector<uint8_t> buff((size_t)size);
                do {
                    //fromFile->Read(buff.data(), kChunkSize);
                    //toFile->Write(buff.data(), size);
                }
                while (size == kChunkSize);
                result = true;
            }
        }
        
        return result;
    }

    bool NativeFileSystem::RenameFile(const FileInfoPtr& src, const FileInfoPtr& dest) {
        if (!IsReadOnly()) {
            return false;
        }
        
        bool result = false;
        
        FilePtr fromFile = FindFile(src);
        FilePtr toFile = FindFile(dest);
        if (fromFile && !toFile) {
            FileInfoPtr toInfo = std::make_shared<FileInfo>(BasePath(), dest->AbsolutePath(), false);
            if (rename(fromFile->FileInfo()->AbsolutePath().c_str(), toInfo->AbsolutePath().c_str())) {
                file_list.erase(fromFile);
                toFile = OpenFile(dest, FileMode::READ_BINARY);
                if (toFile) {
                    result = true;
                    toFile->Close();
                }
            }
        }
        
        return result;
    }

    bool NativeFileSystem::IsFileExists(const FileInfoPtr& file_path) const {
        auto extra_info = std::make_shared<FileInfo>(BasePath() + file_path->AbsolutePath());
        return (FindFile(extra_info) != nullptr);
    }

    bool NativeFileSystem::IsFile(const FileInfoPtr& file_path) const {
        FilePtr file = FindFile(file_path);
        if (file) {
            return !file->FileInfo()->IsDir();
        }
        return false;
    }

    bool NativeFileSystem::IsDir(const FileInfoPtr& dir_path) const {
        FilePtr file = FindFile(dir_path);
        if (file) {
            return file->FileInfo()->IsDir();
        }
        return false;
    }

    bool NativeFileSystem::IsReadOnly() const {
        if (!IsInitialized()) {
            return true;
        }
        
        struct stat file_stat;
        if (stat(BasePath().c_str(), &file_stat) < 0) {
            return false;
        }
#ifdef RK_WINDOWS
        return (file_stat.st_mode & _S_IWRITE);
#else
        return (file_stat.st_mode & S_IWUSR);
#endif
    }

    FilePtr NativeFileSystem::FindFile(const FileInfoPtr& fileInfo) const {
        FileList::const_iterator it = std::find_if(file_list.begin(), file_list.end(), [&](FilePtr file) {
            return file->FileInfo() == fileInfo;
        });
        if (it != file_list.end()) {
            return *it;
        }
        return nullptr;
    }

    void NativeFileSystem::BuildFilelist(SDir* dir, const std::string& base, FileList& out) {
        auto basePath = base;
        if (!EndsWith(basePath, "/")) {
            basePath += "/";
        }
        struct dirent* ent;
        while((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            std::string filepath = basePath + filename;
            SDir* childDir = static_cast<SDir*>(opendir(filepath.c_str()));
            bool isDotOrDotDot = EndsWith(filename, ".") && childDir;
            if (childDir && !isDotOrDotDot) {
                filename += "/";
            }
            FileInfoPtr fileInfo = std::make_shared<FileInfo>(basePath, filename, childDir != nullptr);
            if(!FindFile(fileInfo)) {
                FilePtr file(new NativeFile(fileInfo));
                out.insert(file);
            }
            if(childDir) {
                if(!isDotOrDotDot) {
                    BuildFilelist(childDir, (childDir ? filepath : basePath), out);
                }
                closedir(childDir);
            }
        }
    }
}
