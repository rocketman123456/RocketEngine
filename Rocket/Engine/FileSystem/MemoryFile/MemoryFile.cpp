#include "FileSystem/MemoryFile/MemoryFile.h"

#include <algorithm>

namespace Rocket {
    void MemoryFile::Open(int32_t mode) {
        // Check Re-open case
        if(IsOpened() && this->mode & mode != 0) {
            RK_WARN(File, "Reopen File");
            Seek(0, File::FILE_BEGIN);
            return;
        }

        this->mode = mode;
        this->seek_pos = 0;
        this->is_read_only = true;

        if (mode & File::WRITE_BINARY) {
            is_read_only = false;
        }
        if (mode & File::WRITE_TEXT) {
            is_read_only = false;
        }
        if (mode & File::APPEND) {
            is_read_only = false;
            seek_pos = Size() > 0 ? Size() - 1 : 0;
        }
        if (mode & File::TRUNCATE) {
            if(file_data.size() > 0) delete [] file_data.data();
            file_data = {nullptr, std::size_t(0)};
        }

        is_opened = true;
    }

    void MemoryFile::Close() {
        if(file_data.size() > 0) delete [] file_data.data();
        file_data = {nullptr, std::size_t(0)};
        is_read_only = true;
        is_opened = false;
        seek_pos = 0;
    }

    std::size_t MemoryFile::Seek(std::size_t offset, FileOrigin origin) {
        if (!IsOpened()) {
            return 0;
        }
        if (origin == File::FILE_BEGIN) {
            seek_pos = offset;
        } else if (origin == File::FILE_END) {
            seek_pos = Size() - offset;
        } else {
            seek_pos += offset;
        }
        seek_pos = std::max(seek_pos, std::size_t(0));
        seek_pos = std::min(seek_pos, Size() - 1);
        return Tell();
    }

    std::size_t MemoryFile::Tell() {
        return seek_pos;
    }

    gsl::span<gsl::byte> MemoryFile::Read(std::size_t size) {
        if (!IsOpened()) {
            return gsl::span<gsl::byte>(nullptr, std::size_t(0));
        }

        gsl::byte* buffer = nullptr;
        std::size_t buffer_size = Size() - Tell();
        std::size_t max_size = std::min(size, buffer_size);
        if (max_size > 0) {
            buffer = new gsl::byte[max_size];
            std::memcpy(buffer, file_data.data(), max_size);
            return {buffer, max_size};
        } else {
            return {buffer, std::size_t(0)};
        }
    }

    std::size_t MemoryFile::Write(gsl::span<gsl::byte> data) {
        if (!IsOpened() || IsReadOnly()) {
            return 0;
        }
        std::size_t buffer_size = Size() - Tell();
        if(data.size() > buffer_size) {
            // Resize Buffer Area, Make New one First
            gsl::byte* temp_data = new gsl::byte[file_data.size() + data.size() - buffer_size];
            gsl::span<gsl::byte> temp{temp_data, static_cast<std::size_t>(file_data.size() + data.size() - buffer_size)};
            // Copy Data
            std::memcpy(temp.data(), file_data.data(), file_data.size());
            // Delete Old Buffer Area
            delete [] file_data.data();
            // Update Buffer
            file_data = std::move(temp);
        }
        // Copy Needed Data
        std::memcpy(file_data.data() + Tell(), data.data(), data.size());
    }
}
