#include "FileSystem/ZipFile.h"
#include "Log/Log.h"

#include <exception>

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

void Display64BitsSize(ZPOS64_T n, int size_char) {
    /* to avoid compatibility problem , we do here the conversion */
    char number[21];
    int offset = 19;
    int pos_string = 19;
    number[20] = 0;
    for (;;) {
        number[offset] = (char)((n % 10) + '0');
        if (number[offset] != '0')
            pos_string = offset;
        n /= 10;
        if (offset == 0)
            break;
        offset--;
    }
    {
        int size_display_string = 19 - pos_string;
        while (size_char > size_display_string) {
            size_char--;
            printf(" ");
        }
    }

    printf("%s", &number[pos_string]);
}

namespace Rocket {
    ZipAsyncFileOperation::ZipAsyncFileOperation(const ZipAsyncFileOperation& other) : file_(other.file_) {
        int32_t temp = other.overlapped_;
        overlapped_ = temp;
        overlapped_++; 
    }

    ZipAsyncFileOperation& ZipAsyncFileOperation::operator=(const ZipAsyncFileOperation& other) {
        if (this != &other) {
            file_ = other.file_;
            int32_t temp = other.overlapped_;
            overlapped_ = temp;
            overlapped_++;
        }
        return *this;
    }

    int32_t ZipFile::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = path + file_name;
        file_.file_pointer = unzOpen64(file_.full_name.c_str());
        if(file_.file_pointer == nullptr) {
            RK_ERROR(File, "Open Zip File {} Error", file_.full_name);
            throw std::runtime_error("Open Zip File Error");
        }
        int32_t err = 0;
        unz_global_info64 global_info;
        err = unzGetGlobalInfo64(file_.file_pointer, &global_info);
        if(err != UNZ_OK) {
            RK_ERROR(File, "Error {} with ZipFile in unzGetGlobalInfo", err);
        }
        printf("  Length  Method     Size Ratio   Date    Time   CRC-32     Name\n");
        printf("  ------  ------     ---- -----   ----    ----   ------     ----\n");
        for (int32_t i = 0; i < global_info.number_entry; i++) {
            char filename_inzip[256];
            unz_file_info64 file_info;
            uLong ratio = 0;
            const char *string_method;
            char charCrypt = ' ';
            err = unzGetCurrentFileInfo64(file_.file_pointer, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);
            if (err != UNZ_OK) {
                RK_ERROR(File, "Error {} with ZipFile in unzGetCurrentFileInfo", err);
                break;
            }

            if (file_info.uncompressed_size > 0)
                ratio = (uLong)((file_info.compressed_size * 100) / file_info.uncompressed_size);

            /* display a '*' if the file is crypted */
            if ((file_info.flag & 1) != 0)
                charCrypt = '*';

            if (file_info.compression_method == 0)
                string_method = "Stored";
            else if (file_info.compression_method == Z_DEFLATED) {
                uInt iLevel = (uInt)((file_info.flag & 0x6) / 2);
                if (iLevel == 0)
                    string_method = "Defl:N";
                else if (iLevel == 1)
                    string_method = "Defl:X";
                else if ((iLevel == 2) || (iLevel == 3))
                    string_method = "Defl:F"; /* 2:fast , 3 : extra fast*/
            }
            else if (file_info.compression_method == Z_BZIP2ED) {
                string_method = "BZip2 ";
            }
            else
                string_method = "Unkn. ";

            Display64BitsSize(file_info.uncompressed_size, 7);
            printf("  %6s%c", string_method, charCrypt);
            Display64BitsSize(file_info.compressed_size, 7);
            printf(" %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lx   %s\n",
                ratio,
                (uLong)file_info.tmu_date.tm_mon + 1,
                (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour, (uLong)file_info.tmu_date.tm_min,
                (uLong)file_info.crc, filename_inzip);
            if ((i + 1) < global_info.number_entry)
            {
                err = unzGoToNextFile(file_.file_pointer);
                if (err != UNZ_OK)
                {
                    printf("error %d with zipfile in unzGoToNextFile\n", err);
                    break;
                }
            }
        }
        return 0;
    }

    void ZipFile::Finalize() {
        unzClose(file_.file_pointer);
        content_.clear();
    }
}
