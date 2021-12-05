#include "FileSystem/NativeFile/NativeFile.h"
#include "Log/Log.h"

namespace Rocket {
    void NativeFile::Open(int32_t mode) {
        // Check Re-open case
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

        stream.open(file_info->AbsolutePath().c_str(), open_mode);

        // Calculate File Size
        if (IsOpened()) {
            auto cur_pos = Tell();
            Seek(0, FileMode::END);
            file_size = Tell();
            Seek(cur_pos, FileMode::BEGIN);
        }
    }

    void NativeFile::Close() {
        stream.close();
    }

    std::size_t NativeFile::Seek(std::size_t offset, FileMode::FileOrigin origin) {
        if (!IsOpened()) {
            return 0;
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
        return static_cast<std::size_t>(stream.tellg());
    }

    gsl::span<gsl::byte> NativeFile::Read(std::size_t size) {
        if (!IsOpened()) {
            return gsl::span<gsl::byte>(nullptr, std::size_t(0));
        }
        gsl::byte* data = nullptr;
        stream.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
        if (stream) {
            return gsl::span<gsl::byte>(data, size);
        }
        return gsl::span<gsl::byte>(data, static_cast<std::size_t>(stream.gcount()));
    }

    std::size_t NativeFile::Write(gsl::span<gsl::byte> data) {
        if (!IsOpened() || IsReadOnly()) {
            return 0;
        }
        stream.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
        if (stream) {
            return data.size();
        }
        return static_cast<std::size_t>(stream.gcount());
    }
}
