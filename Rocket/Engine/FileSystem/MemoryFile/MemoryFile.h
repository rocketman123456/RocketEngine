#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/FileInfo.h"
#include "FileSystem/Basic/FileBuffer.h"

namespace Rocket {
    class MemoryFile : _implements_ File {
    public:
        MemoryFile(const FileInfoPtr& info);
        virtual ~MemoryFile();

        FileInfoPtr GetFileInfo() const final {}
        std::size_t Size() const final;
        bool IsReadOnly() const final;
        bool IsOpened() const final;
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
        int32_t mode = 0;
        bool is_read_only = false;
    };
}
