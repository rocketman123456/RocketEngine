#include "FileSystem/NativeFile/NativeFile.h"
#include "Log/Log.h"

#include <filesystem>

namespace Rocket {
    //NativeFile::NativeFile(const VirtualNodePtr& vnode_, const std::string& path_, const std::string& alias_path_)
    //    : vnode(vnode_), file_path(path_), alias_path(alias_path_) {}
    
    NativeFile::NativeFile(const std::string& path_, const std::string& alias_path_)
        : file_path(path_), alias_path(alias_path_) {}

    void NativeFile::Open(int32_t mode) {
        if(IsOpened() && (this->mode & mode) != 0) {
            RK_WARN(File, "Reopen File");
            Seek(0, FileMode::BEGIN);
            return;
        }

        this->mode = mode;
        this->is_read_only = true;

        std::ios_base::openmode open_mode = (std::ios_base::openmode)0x00;
        if (mode & FileMode::READ_BINARY) {
            open_mode |= std::fstream::in;
            open_mode |= std::fstream::binary;
        }
        if (mode & FileMode::WRITE_BINARY) {
            is_read_only = false;
            open_mode |= std::fstream::out;
            open_mode |= std::fstream::binary;
        }
        if (mode & FileMode::READ_TEXT) {
            open_mode |= std::fstream::in;
        }
        if (mode & FileMode::WRITE_TEXT) {
            is_read_only = false;
            open_mode |= std::fstream::out;
        }
        if (mode & FileMode::APPEND) {
            is_read_only = false;
            open_mode |= std::fstream::app;
        }
        if (mode & FileMode::TRUNCATE) {
            open_mode |= std::fstream::trunc;
        }

        // Check in filesystem to assure is_read_only
        if(!is_read_only) {
            auto perm = std::filesystem::status(file_path).permissions();
            if((perm & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
                //(perm & std::filesystem::perms::group_write) == std::filesystem::perms::none) &&
                //(perm & std::filesystem::perms::others_write) == std::filesystem::perms::none)) {
                is_read_only = true;
            }
        }

        RK_TRACE(File, "Native Path: {}, {}", file_path, alias_path);
        stream.open(file_path, open_mode);

        // Calculate File Size
        UpdateSize();
    }

    void NativeFile::Close() {
        stream.close();
        file_size = std::size_t(0);
    }

    void NativeFile::UpdateSize() {
        if (IsOpened()) {
#if 0
            auto cur_pos = Tell();
            Seek(0, FileMode::END);
            file_size = Tell();
            Seek(cur_pos, FileMode::BEGIN);
#else
            std::filesystem::path path = this->file_path;
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

    std::size_t NativeFile::Seek(std::size_t offset, FileMode::FileOrigin origin) {
        if (!IsOpened()) {
            return std::size_t(0);
        }
        std::ios_base::seekdir way;
        if (origin == FileMode::BEGIN) {
            way = std::ios_base::beg;
        } else if (origin == FileMode::END) {
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

    std::size_t NativeFile::Read(gsl::span<gsl::byte>& buffer) {
        if (!IsOpened()) {
            return std::size_t(0);
        }
        stream.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        if (stream) {
            return buffer.size();
        }
        return static_cast<std::size_t>(stream.gcount());
    }

    std::size_t NativeFile::Write(gsl::span<gsl::byte> data) {
        if (!IsOpened() || IsReadOnly()) {
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
