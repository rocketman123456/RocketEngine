#pragma once
#include "FileSystem/Basic/VirtualNode.h"

#include <gsl/gsl>

namespace Rocket {
    namespace FileEnum {
        enum Mode {
            READ_BINARY = 1 << 0,
            WRITE_BINARY = 1 << 1,
            READWRITE_BINARY = READ_BINARY | WRITE_BINARY,
            READ_TEXT = 1 << 2,
            WRITE_TEXT = 1 << 3,
            READWRITE_TEXT = READ_TEXT | WRITE_TEXT,
            APPEND = 1 << 4,
            TRUNCATE = 1 << 5,
        };

        enum Origin {
            BEGIN = 0,
            END = 1,
            SET = 2,
        };
    }

    using FileByte = std::byte;
    using FileBuffer = gsl::span<FileByte>;

    // File Handle
    _Interface_ File {
    public:
        virtual ~File() = default;

        [[nodiscard]] virtual VirtualNodePtr VNode() const = 0;
        [[nodiscard]] virtual std::string VirtualPath() const = 0;
        [[nodiscard]] virtual std::string RealPath() const = 0;
        virtual void Open(int32_t mode) = 0;
        virtual void Close() = 0;
        [[nodiscard]] virtual bool IsOpened() const = 0;
        [[nodiscard]] virtual bool IsReadOnly() const = 0;
        [[nodiscard]] virtual std::size_t Size() const = 0;
        [[nodiscard]] virtual std::size_t Seek(std::size_t offset, FileEnum::Origin origin) = 0;
        [[nodiscard]] virtual std::size_t Tell() = 0;
        // Should Pre-Allocate Memory
        [[nodiscard]] virtual std::size_t Read(FileBuffer* data) = 0;
        [[nodiscard]] virtual std::size_t Write(const FileBuffer& data) = 0;
    };

    CLASS_PTR(File);
}
