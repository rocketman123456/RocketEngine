#include "FileSystem/NativeFile/NativeFile.h"
#include "Log/Log.h"

#include <filesystem>

namespace Rocket {
    NativeFile::NativeFile(const VirtualNodePtr& vnode_) : vnode(vnode_) {
        // TODO
        virtual_path = vnode->path + vnode->name;
        //auto fs_real_path = vnode->vblock->file_system->RealPath();
        //auto fs_virtual_path = vnode->vblock->file_system->VirtualPath();
        //real_path = fs_real_path + vnode->path.substr(fs_virtual_path.size()) + vnode->name
    }
    NativeFile::NativeFile(const std::string& path_, const std::string& virtual_path_)
        : real_path(path_), virtual_path(virtual_path_) {}
    NativeFile::NativeFile(const std::string& path_)
        : real_path(path_) {}

    void NativeFile::Open(int32_t mode) {
        if(IsOpened() && (this->mode & mode) != 0) {
            RK_WARN(File, "Reopen Native File {}, {}", virtual_path, real_path);
            Seek(0, FileEnum::BEGIN);
            return;
        }
        this->mode = mode;
        std::ios_base::openmode open_mode = (std::ios_base::openmode)0x00;
        if (mode & FileEnum::READ_BINARY) {
            is_read_only = true;
            open_mode |= std::fstream::in;
            open_mode |= std::fstream::binary;
        }
        if (mode & FileEnum::WRITE_BINARY) {
            is_read_only = false;
            open_mode |= std::fstream::out;
            open_mode |= std::fstream::binary;
        }
        if (mode & FileEnum::READ_TEXT) {
            is_read_only = true;
            open_mode |= std::fstream::in; 
        }
        if (mode & FileEnum::WRITE_TEXT) {
            is_read_only = false;
            open_mode |= std::fstream::out;
        }
        if (mode & FileEnum::APPEND) {
            is_read_only = false;
            open_mode |= std::fstream::app;
        }
        if (mode & FileEnum::TRUNCATE) {
            is_read_only = false;
            open_mode |= std::fstream::trunc; 
        }
        // Check in filesystem to assure is_read_only
        if(!is_read_only) {
            auto perm = std::filesystem::status(real_path).permissions();
            // Check Owner's Permission
            if((perm & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
                is_read_only = true;
            }
        }
        RK_TRACE(File, "Native Path: {}, {}", virtual_path, real_path);
        stream.open(real_path, open_mode);
        // Calculate File Size
        UpdateSize();
    }

    void NativeFile::Close() {
        stream.close();
        file_size = std::size_t(0);
    }

    void NativeFile::UpdateSize() {
        if (IsOpened()) {
#if 1
            auto cur_pos = Tell();
            Seek(0, FileEnum::END);
            file_size = Tell();
            Seek(cur_pos, FileEnum::BEGIN);
#else
            std::filesystem::path path = this->real_path;
            auto err = std::error_code{};
            auto filesize = std::filesystem::file_size(path, err);
            if (filesize != static_cast<uintmax_t>(-1))
                file_size = filesize;
            else 
                file_size = 0;
#endif
        } else {
            file_size = std::size_t(0);
        }
    }

    std::size_t NativeFile::Seek(std::size_t offset, FileEnum::Origin origin) {
        if (!IsOpened()) {
            return std::size_t(0);
        }
        std::ios_base::seekdir way;
        if (origin == FileEnum::BEGIN) {
            way = std::ios_base::beg;
        } else if (origin == FileEnum::END) {
            way = std::ios_base::end;
        } else {
            way = std::ios_base::cur;
        }
        stream.seekg(offset, way);
        stream.seekp(offset, way);
        return Tell();
    }

    std::size_t NativeFile::Tell() {
        if(!IsOpened()) {
            return std::size_t(0);
        } else {
            return static_cast<std::size_t>(stream.tellg());
        }
    }

    std::size_t NativeFile::Read(FileBuffer& buffer) {
        if (!IsOpened()) {
            return std::size_t(0);
        }
        stream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        if (stream) {
            return buffer.size();
        }
        return static_cast<std::size_t>(stream.gcount());
    }

    std::size_t NativeFile::Write(const FileBuffer& data) {
        if (!IsOpened() || IsReadOnly()) {
            RK_WARN(File, "Write to Read Only File {}, {}", virtual_path, real_path);
            return 0;
        }
        stream.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        UpdateSize();
        if (stream) {
            return data.size();
        }
        return static_cast<std::size_t>(stream.gcount());
    }
}
