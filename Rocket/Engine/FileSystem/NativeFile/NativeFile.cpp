#include "FileSystem/NativeFile/NativeFile.h"
#include "Log/Log.h"

namespace Rocket {
    //std::size_t NativeFile::Size() const {
        // if (IsOpened()) {
        //     uint64_t cur_pos = Tell();
        //     Seek(0, FILE_END);
        //     file_size = Tell();
        //     Seek(cur_pos, FILE_BEGIN);
        //     return size;
        // }
    //}

    void NativeFile::Open(int32_t mode) {
        // Check Re-open case
        if(IsOpened() && this->mode & mode != 0) {
            RK_WARN(File, "Reopen File");
            Seek(0, File::FILE_BEGIN);
            return;
        }

        this->mode = mode;
        this->is_read_only = true;

        std::ios_base::openmode open_mode = (std::ios_base::openmode)0x00;
        if (mode & File::READ_BINARY) {
            open_mode |= std::fstream::in;
            open_mode |= std::fstream::binary;
        }
        if (mode & File::WRITE_BINARY) {
            is_read_only = false;
            open_mode |= std::fstream::out;
            open_mode |= std::fstream::binary;
        }
        if (mode & File::READ_TEXT) {
            open_mode |= std::fstream::in;
        }
        if (mode & File::WRITE_TEXT) {
            is_read_only = false;
            open_mode |= std::fstream::out;
        }
        if (mode & File::APPEND) {
            is_read_only = false;
            open_mode |= std::fstream::app;
        }
        if (mode & File::TRUNCATE) {
            open_mode |= std::fstream::trunc;
        }

        stream.open(file_info->AbsolutePath().c_str(), open_mode);
    }

    void NativeFile::Close() {
        stream.close();
    }

    std::size_t NativeFile::Seek(std::size_t offset, FileOrigin origin) {
        if (!IsOpened()) {
            return 0;
        }
        
        std::ios_base::seekdir way;
        if (origin == File::FILE_BEGIN) {
            way = std::ios_base::beg;
        } else if (origin == File::FILE_END) {
            way = std::ios_base::end;
        } else {
            way = std::ios_base::cur;
        }
        
        stream.seekg(offset, way);
        stream.seekp(offset, way);
        
        return Tell();
    }

    std::size_t NativeFile::Tell() {
        return static_cast<std::size_t>(stream.tellg());
    }

    gsl::span<gsl::byte> NativeFile::Read(std::size_t size) {
        if (!IsOpened()) {
            return gsl::span<gsl::byte>(nullptr, static_cast<std::size_t>(0));
        }
        gsl::byte* data = nullptr;
        stream.read(reinterpret_cast<char*>(data), (std::streamsize)size);
        if (stream) {
            return gsl::span<gsl::byte>(data, size);
        }
        return gsl::span<gsl::byte>(data, stream.gcount());
    }

    std::size_t NativeFile::Write(gsl::span<gsl::byte> data, std::size_t size) {
        if (!IsOpened() || IsReadOnly()) {
            return 0;
        }
        stream.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(size));
        if (stream) {
            return size;
        }
        return static_cast<uint64_t>(stream.gcount());
    }
}
