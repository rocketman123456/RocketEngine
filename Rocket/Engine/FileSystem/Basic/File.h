#pragma once
#include "FileSystem/Basic/FileInfo.h"
#include "FileSystem/Basic/FileBuffer.h"

namespace Rocket {
    class File {
    public:
        enum FileOperateMode {
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
            FILE_BEGIN = 1 << 0,
            FILE_END = 1 << 1,
            FILE_SET = 1 << 2,
        };

    public:
        File() = default;
        virtual ~File() = default();

        virtual FileInfoPtr GetFileInfo() const = 0;
        virtual std::size_t Size() const = 0;
        virtual bool IsReadOnly() const = 0;
        virtual bool IsOpened() const = 0;
        virtual void Open(int32_t mode) = 0;
        virtual void Close() = 0;
        virtual std::size_t Seek(std::size_t offset, FileOrigin origin) = 0;
        virtual std::size_t SeekToEnd(void) = 0;
        virtual std::size_t Tell() = 0;

        virtual std::size_t Read(uint8_t* buffer, std::size_t size) = 0;
        virtual std::size_t Write(const uint8_t* buffer, std::size_t size) = 0;

        template<typename T>
        std::size_t Read(T& value) {
            return (Read(&value, sizeof(value)) == sizeof(value)) ? sizeof(value) : 0;
        }

        template<typename T>
        std::size_t Write(const T& value) {
            return (Write(&value, sizeof(value)) == sizeof(value)) ? sizeof(value) : 0;
        }

    // private:
        //FileInfoPtr file_info_ = nullptr;
        //FileBufferPtr file_buffer_ = nullptr;
    };
}