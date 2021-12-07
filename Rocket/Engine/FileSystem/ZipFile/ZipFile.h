#pragma once
#include "FileSystem/Basic/File.h"
#include "FileSystem/ZipFile/ZipInfo.h"

namespace Rocket {
    class ZipFile : _implements_ File {
    public:
        ZipFile(const FileInfoPtr& info);
        virtual ~ZipFile();

        inline FileInfoPtr FileInfo() const final { return file_info; }
        inline ZipInfoPtr ZipInfo() const { return zip_info; }
        
        std::size_t Size() const final;
        bool IsReadOnly() const final;
        bool IsOpened() const final;
        void Open(int32_t mode) final;
        void Close() final;
        std::size_t Seek(std::size_t offset, FileMode::FileOrigin origin) final;
        std::size_t Tell() final;

        std::size_t Read(gsl::span<gsl::byte>& data) final;
        std::size_t Write(gsl::span<gsl::byte> data) final;
    private:
        FileInfoPtr file_info = nullptr;
        ZipInfoPtr zip_info = nullptr;
        std::size_t seek_pos = 0;
        int32_t mode = 0;
        bool is_read_only = false;
        bool is_opened = false;
        bool is_changed = false;
    };

    CLASS_PTR(ZipFile);
}
