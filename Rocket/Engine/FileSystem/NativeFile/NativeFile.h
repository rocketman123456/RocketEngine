#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/FileInfo.h"
#include "FileSystem/Basic/FileBuffer.h"

#include <fstream>

namespace Rocket {
    class NativeFile : _implements_ File {
    public:
        NativeFile(const FileInfoPtr& info) : file_info(info) {}
        virtual ~NativeFile() = default;//{ Close(); }

        inline FileInfoPtr GetFileInfo() const final { return file_info; }
        inline bool IsReadOnly() const final { return is_read_only; }
        inline bool IsOpened() const final { return stream.is_open(); }
        inline std::size_t Size() const final { return file_size; }

        void Open(int32_t mode) final;
        void Close() final;
        std::size_t Seek(std::size_t offset, FileOrigin origin) final;
        std::size_t Tell() final;

        gsl::span<gsl::byte> Read(std::size_t size) final;
        std::size_t Write(gsl::span<gsl::byte> data, std::size_t size) final;
        //std::size_t Read(uint8_t* buffer, std::size_t size) final;
        //std::size_t Write(const uint8_t* buffer, std::size_t size) final;
    private:
        FileInfoPtr file_info = nullptr;
        std::size_t file_size = 0;;
        int32_t mode = 0;
        bool is_read_only = false;
        std::fstream stream;
    };
}
