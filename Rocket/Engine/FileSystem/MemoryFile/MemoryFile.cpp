#include "FileSystem/MemoryFile/MemoryFile.h"
#include "FileSystem/Basic/VirtualUtils.h"

#include <algorithm>
#include <cassert>

namespace Rocket {
    MemoryFile::MemoryFile(const VirtualNodePtr& vnode_) 
        : vnode(vnode_), real_path(GetVirtualPath(vnode)), virtual_path(GetRealPath(vnode)) {}
    MemoryFile::MemoryFile(const std::string& path_, const std::string& virtual_path_)
        : real_path(path_), virtual_path(virtual_path_) {}
    MemoryFile::MemoryFile(const std::string& path_)
        : real_path(path_) {}

    void MemoryFile::Open(int32_t mode) {
        // Check Re-open case
        if(IsOpened() && (this->mode & mode) != 0) {
            RK_WARN(File, "Reopen Memory File {}, {}", virtual_path, real_path);
            Seek(0, FileEnum::BEGIN);
            return;
        }
        this->mode = mode;
        this->seek_pos = 0;
        this->is_read_only = true;
        if (mode & FileEnum::WRITE_BINARY) {
            is_read_only = false;
        }
        if (mode & FileEnum::WRITE_TEXT) {
            is_read_only = false;
        }
        if (mode & FileEnum::APPEND) {
            is_read_only = false;
            seek_pos = Size() > 0 ? Size() - 1 : 0;
        }
        if (mode & FileEnum::TRUNCATE) {
            is_read_only = false;
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

    std::size_t MemoryFile::Seek(std::size_t offset, FileEnum::Origin origin) {
        if (!IsOpened()) { return std::size_t(0); }
        if (origin == FileEnum::BEGIN) {
            seek_pos = offset;
        } else if (origin == FileEnum::END) {
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

    std::size_t MemoryFile::Read(FileBuffer* buffer) {
        assert(buffer != nullptr);
        if (!IsOpened()) { return std::size_t(0); }
        // Copy Specify Memory Area
        std::size_t buffer_size = Size() - Tell();
        std::size_t max_size = std::min(buffer->size(), buffer_size);
        if (max_size > 0) {
            std::memcpy(buffer->data(), file_data.data(), max_size);
            return max_size;
        } else {
            return std::size_t(0);
        }
    }

    std::size_t MemoryFile::Write(const FileBuffer& data) {
        if (!IsOpened() || IsReadOnly()) { 
            RK_WARN(File, "Write to Read Only File {}, {}", virtual_path, real_path);
            return 0; 
        }
        std::size_t buffer_size = Size() - Tell();
        if(data.size() > buffer_size) {
            // Resize Buffer Area, Make New one First
            FileByte* temp_data = new FileByte[file_data.size() + data.size() - buffer_size];
            FileBuffer temp{temp_data, static_cast<std::size_t>(file_data.size() + data.size() - buffer_size)};
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
