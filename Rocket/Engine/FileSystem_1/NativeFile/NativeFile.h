#pragma once
#include "FileSystem/Basic/File.h"

#include <fstream>

namespace Rocket {
    class NativeFile : _implements_ File {
    public:
        NativeFile(const FileInfoPtr& info) : file_info(info) {}
        virtual ~NativeFile() = default;//{ Close(); }

        inline FileInfoPtr FileInfo() const final { return file_info; }
        inline bool IsReadOnly() const final { return is_read_only; }
        inline bool IsOpened() const final { return stream.is_open(); }
        inline std::size_t Size() const final { return file_size; }

        void Open(int32_t mode) final;
        void Close() final;
        std::size_t Seek(std::size_t offset, FileMode::FileOrigin origin) final;
        std::size_t Tell() final;

        std::size_t Read(gsl::span<gsl::byte>& buffer) final;
        std::size_t Write(gsl::span<gsl::byte> data) final;
    private:
        void UpdateSize();
    private:
        FileInfoPtr file_info = nullptr;
        std::size_t file_size = 0;
        int32_t mode = 0;
        bool is_read_only = false;
        std::fstream stream;
    };

    CLASS_PTR(NativeFile);
}