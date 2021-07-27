#pragma once
#include "FileSystem/File.h"
#include "FileSystem/OsFile.h"

#include <cstdint>
#include <string>
#include <functional>
#include <cstdlib>
#include <atomic>
#include <unordered_map>

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

namespace Rocket {
    class ZipAsyncFileOperation {
    public:
        ZipAsyncFileOperation(const FileHandle& file, size_t position);
        ZipAsyncFileOperation(const ZipAsyncFileOperation& other);
        ZipAsyncFileOperation& operator=(const ZipAsyncFileOperation& other);
        ~ZipAsyncFileOperation() = default;
        /// Returns whether or not the asynchronous operation has finished
        bool HasFinished() const;
        /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
        size_t WaitUntilFinished() const;
        /// Cancels the asynchronous operation
        void Cancel();
    private:
        FileHandle file_;
        std::atomic_int32_t overlapped_;
    };
    
    using ZipContentsMap = std::unordered_map<std::string, int>;
    using byte = uint8_t;
    using word = uint16_t;
    using dword = uint32_t;

#pragma pack(1)
    struct ZipFileInfo {
        enum {
            SIGNATURE = 0x04034b50,
        };
        dword   sig;
        word    version;
        word    flag;
        word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
        word    modTime;
        word    modDate;
        dword   crc32;
        dword   cSize;
        dword   ucSize;
        word    fnameLen;         // Filename string follows header.
        word    xtraLen;          // Extra field follows filename.
    };
#pragma pack()
#pragma pack(1)
    struct ZipContentInfo {
        enum {
            SIGNATURE = 0x06054b50
        };
        dword   sig;
        word    nDisk;
        word    nStartDisk;
        word    nDirEntries;
        word    totalDirEntries;
        dword   dirSize;
        dword   dirOffset;
        word    cmntLen;
    };
#pragma pack()
#pragma pack(1)
    struct ZipContentFileInfo {
        enum {
            SIGNATURE   = 0x02014b50
        };
        dword   sig;
        word    verMade;
        word    verNeeded;
        word    flag;
        word    compression;      // COMP_xxxx
        word    modTime;
        word    modDate;
        dword   crc32;
        dword   cSize;            // Compressed size
        dword   ucSize;           // Uncompressed size
        word    fnameLen;         // Filename string follows header.
        word    xtraLen;          // Extra field follows filename.
        word    cmntLen;          // Comment field follows extra field.
        word    diskStart;
        word    intAttr;
        dword   extAttr;
        dword   hdrOffset;

        char *GetName   () const { return (char *)(this + 1);   }
        char *GetExtra  () const { return GetName() + fnameLen; }
        char *GetComment() const { return GetExtra() + xtraLen; }
    };
#pragma pack()

    class ZipFile : public OsFile {
    public:
        ZipFile() = default;
        virtual ~ZipFile() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;

        int32_t GetNumFiles() const;
        std::string GetFilename(int32_t i) const;	
        int32_t GetFileLen(int32_t i) const;
        bool ReadFile(int32_t i, void* buffer);
        int32_t Find(const std::string& path) const;

        /// Asynchronously reads from the file into a buffer
        ZipAsyncFileOperation ReadAsync(FileBuffer& buffer, std::size_t length);
        /// Asynchronously writes from a buffer into the file
        ZipAsyncFileOperation WriteAsync(FileBuffer& buffer, std::size_t length);
        // Added to show multi-threaded decompression
        bool ReadLargeFile(int32_t i, void* buffer, std::function<void(int32_t, bool&)> call_back);

    private:
        ZipContentsMap content_;
    };
}
