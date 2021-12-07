#pragma once
#include "FileSystem/Basic/FileInfo.h"
// #include "FileSystem/Basic/FileBuffer.h"

#include <gsl/gsl>
#include <future>

namespace Rocket {
    namespace FileMode {
        enum FileOperate {
            READ_BINARY = 1 << 0,
            WRITE_BINARY = 1 << 1,
            READWRITE_BINARY = READ_BINARY | WRITE_BINARY,
            READ_TEXT = 1 << 2,
            WRITE_TEXT = 1 << 3,
            READWRITE_TEXT = READ_TEXT | WRITE_TEXT,
            APPEND = 1 << 4,
            TRUNCATE = 1 << 5,
        };

        enum FileOrigin {
            BEGIN = 0,
            END = 1,
            SET = 2,
        };
    }

    _Interface_ File {
    public:
        File() = default;
        virtual ~File() = default;

        virtual FileInfoPtr FileInfo() const = 0;
        virtual std::size_t Size() = 0;
        virtual bool IsReadOnly() const = 0;
        virtual bool IsOpened() const = 0;
        virtual void Open(int32_t mode) = 0;
        virtual void Close() = 0;
        virtual std::size_t Seek(std::size_t offset, FileMode::FileOrigin origin) = 0;
        virtual std::size_t Tell() = 0;

        // TODO : change to file buffer
        virtual std::size_t Read(gsl::span<gsl::byte>& data) = 0;
        virtual std::size_t Write(gsl::span<gsl::byte> data) = 0;

        // TODO : add async support
        //virtual std::future<gsl::span<gsl::byte>> ReadAsync(std::size_t size) = 0;
        //virtual std::future<std::size_t> WriteAsync(gsl::span<gsl::byte> data, std::size_t size) = 0;

        // template<typename T>
        // gsl::span<T> Read(T& value) { return Read(sizeof(value)); }
        // template<typename T>
        // std::size_t Write(gsl::span<T> value) { return Write(value); }
    };

    CLASS_PTR(File);
}
