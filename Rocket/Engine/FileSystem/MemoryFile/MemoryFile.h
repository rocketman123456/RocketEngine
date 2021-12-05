#pragma once
#include "FileSystem/Basic/File.h"

namespace Rocket {
    class MemoryFile : _implements_ File {
    public:
        MemoryFile(const FileInfoPtr& info) : file_info(info) {}
        virtual ~MemoryFile() = default;//{ Close(); }

        inline FileInfoPtr FileInfo() const final { return file_info; }
        inline std::size_t Size() const final { return IsOpened() ? file_data.size() : 0; }
        inline bool IsReadOnly() const final { return is_read_only; }
        inline bool IsOpened() const final { return is_opened; }

        void Open(int32_t mode) final;
        void Close() final;
        std::size_t Seek(std::size_t offset, FileMode::FileOrigin origin) final;
        std::size_t Tell() final;

        gsl::span<gsl::byte> Read(std::size_t size) final;
        std::size_t Write(gsl::span<gsl::byte> data) final;
    private:
        FileInfoPtr file_info = nullptr;
        gsl::span<gsl::byte> file_data;
        std::size_t seek_pos = 0;
        int32_t mode = 0;
        bool is_read_only = false;
        bool is_opened = false;
    };

    CLASS_PTR(MemoryFile);
}
