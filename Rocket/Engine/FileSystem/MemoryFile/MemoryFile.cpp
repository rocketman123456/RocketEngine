#include "FileSystem/MemoryFile/MemoryFile.h"

#include <algorithm>

namespace Rocket {
    void MemoryFile::Open(int32_t mode) {
        // Check Re-open case
        if(IsOpened() && (this->mode & mode) != 0) {
            RK_WARN(File, "Reopen File");
            Seek(0, FileMode::BEGIN);
            return;
        }

        this->mode = mode;
        this->seek_pos = 0;
        this->is_read_only = true;

        if (mode & FileMode::WRITE_BINARY) {
            is_read_only = false;
        }
        if (mode & FileMode::WRITE_TEXT) {
            is_read_only = false;
        }
        if (mode & FileMode::APPEND) {
            is_read_only = false;
            seek_pos = Size() > 0 ? Size() - 1 : 0;
        }
        if (mode & FileMode::TRUNCATE) {
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

    std::size_t MemoryFile::Seek(std::size_t offset, FileMode::FileOrigin origin) {
        if (!IsOpened()) {
            return 0;
        }
        if (origin == FileMode::BEGIN) {
            seek_pos = offset;
        } else if (origin == FileMode::END) {
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

    std::size_t MemoryFile::Read(gsl::span<gsl::byte>& buffer) {
        if (!IsOpened()) {
            return std::size_t(0);
        }

        // Copy Specify Memory Area
        std::size_t buffer_size = Size() - Tell();
        std::size_t max_size = std::min(buffer.size(), buffer_size);
        if (max_size > 0) {
            std::memcpy(buffer.data(), file_data.data(), max_size);
            return max_size;
        } else {
            return std::size_t(0);
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
        return data.size();
    }
}
