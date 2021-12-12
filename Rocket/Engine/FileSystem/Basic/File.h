#pragma once
#include "FileSystem/Basic/VirtualNode.h"

#include <gsl/gsl>

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

    // File Handle
    _Interface_ File {
    public:
        virtual ~File() = default;

        virtual VirtualNodePtr VNode() const = 0;
        virtual std::string FilePath() const = 0;
        virtual std::string AliasPath() const = 0;
        virtual std::size_t Size() const = 0;
        virtual bool IsReadOnly() const = 0;
        virtual bool IsOpened() const = 0;
        virtual void Open(int32_t mode) = 0;
        virtual void Close() = 0;
        virtual std::size_t Seek(std::size_t offset, FileMode::FileOrigin origin) = 0;
        virtual std::size_t Tell() = 0;

        // Should Pre-Allocate Memory
        virtual std::size_t Read(gsl::span<gsl::byte>& data) = 0;
        virtual std::size_t Write(gsl::span<gsl::byte> data) = 0;
    };

    CLASS_PTR(File);
}
