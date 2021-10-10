#include "FileSystem/ZipFile.h"
#include "Log/Log.h"

#include <exception>

// static int IsLargeFile(const char* filename) {
//     int largeFile = 0;
//     ZPOS64_T pos = 0;
//     FILE* pFile = fopen(filename, "rb");

//     if(pFile != NULL)
//     {
//         int n = fseeko(pFile, 0, SEEK_END);

//         pos = ftello(pFile);
//         printf("File : %s is %lld bytes\n", filename, pos);

//         if(pos >= 0xffffffff)
//             largeFile = 1;

//         fclose(pFile);
//     }

//     return largeFile;
// }

static void Display64BitsSize(ZPOS64_T n, int size_char) {
    // to avoid compatibility problem , we do the conversion here
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
    int32_t ZipFile::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        mode_ = mode;
        file_.file_path = path;
        file_.file_name = file_name;
        file_.full_name = path + file_name;

        return Initialize(file_.full_name, mode_);
    }

    int32_t ZipFile::Initialize(const std::string& path, FileOperateMode mode) {
        mode_ = mode;
        file_.full_name = path;

        int32_t result = 0;
        if(mode_ == FileOperateMode::READ_BINARY)
            result = UnzipInit();
        else if(mode_ == FileOperateMode::WRITE_BINARY)
            result = ZipInit();
        else {
            initialized_ = false;
            return -1;
        }
        
        initialized_ = true;
        return result;
    }

    int32_t ZipFile::UnzipInit() {
        // Open File
        file_.file_pointer = unzOpen64(file_.full_name.c_str());
        if(file_.file_pointer == nullptr) {
            RK_ERROR(File, "Open ZipFile {} Error", file_.full_name);
            throw std::runtime_error("Open ZipFile Error");
        }
        // Get Zip Global Info
        int32_t err = 0;
        err = unzGetGlobalInfo64(file_.file_pointer, &unzip_info_);
        if(err != UNZ_OK) {
            RK_ERROR(File, "Error {} with ZipFile in unzGetGlobalInfo64", err);
        }
#ifdef RK_CONSOLE_LOG
        printf("  Length  Method     Size Ratio   Date    Time   CRC-32     Name\n");
        printf("  ------  ------     ---- -----   ----    ----   ------     ----\n");
#endif
        for (int32_t i = 0; i < unzip_info_.number_entry; i++) {
            // Iterate Through each File Info
            char filename_inzip[256];
            unz_file_info64 file_info;
            unz64_file_pos_s file_pos;
            uint32_t ratio = 0;
            const char *string_method;
            char charCrypt = ' ';
            err = unzGetCurrentFileInfo64(file_.file_pointer, &file_info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);
            if (err != UNZ_OK) {
                RK_ERROR(File, "Error {} with ZipFile in unzGetCurrentFileInfo64", err);
                break;
            }
            err = unzGetFilePos64(file_.file_pointer, &file_pos);
            if(err != UNZ_OK) {
                RK_ERROR(File, "Error {} with ZipFile in unzGetFilePos64", err);
                break;
            }

            if (file_info.uncompressed_size > 0) {
                ratio = (uint32_t)((file_info.compressed_size * 100) / file_info.uncompressed_size);
            }

            // display a '*' if the file is crypted
            if ((file_info.flag & 1) != 0) {
                charCrypt = '*';
            }

            if (file_info.compression_method == 0) {
                string_method = "Stored";
            }
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
            else {
                string_method = "Unkn. ";
            }
#ifdef RK_CONSOLE_LOG
            // display file infos
            Display64BitsSize(file_info.uncompressed_size, 7);
            printf("  %6s%c", string_method, charCrypt);
            Display64BitsSize(file_info.compressed_size, 7);
            printf(" %3u%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lx   %s\n",
                ratio,
                (uLong)file_info.tmu_date.tm_mon + 1,
                (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour, (uLong)file_info.tmu_date.tm_min,
                (uLong)file_info.crc, filename_inzip);
#endif
            // Add Infos to content map
            content_[filename_inzip] = {file_pos, file_info};
            // Get Next File
            if ((i + 1) < unzip_info_.number_entry) {
                err = unzGoToNextFile(file_.file_pointer);
                if (err != UNZ_OK) {
                    RK_ERROR(File, "Error {} with ZipFile in unzGoToNextFile", err);
                    break;
                }
            }
        }
        return 0;
    }

    int32_t ZipFile::ZipInit() {
        file_.file_pointer = zipOpen64(file_.full_name.c_str(), APPEND_STATUS_ADDINZIP);
        //file_.file_pointer = zipOpen64(file_.full_name.c_str(), APPEND_STATUS_CREATE);
        //file_.file_pointer = zipOpen64(file_.full_name.c_str(), APPEND_STATUS_CREATEAFTER);
        if(file_.file_pointer == nullptr) {
            RK_ERROR(File, "Open ZipFile {} Error", file_.full_name);
            throw std::runtime_error("Open ZipFile Error");
        }

        // TODO : finish this

        return 0;
    }

    void ZipFile::Finalize() {
        if(mode_ == FileOperateMode::READ_BINARY)
            unzClose(file_.file_pointer);
        else if(mode_ == FileOperateMode::WRITE_BINARY)
            zipClose(file_.file_pointer, nullptr);
        content_.clear();
    }

    ZipFileInfo ZipFile::Find(const std::string& path) const {
        auto it = content_.find(path);
        if(it == content_.end()) {
            RK_ERROR(File, "Cannot Find {} in ZipFile {}", path, file_.full_name);
            throw std::runtime_error("Cannot Find File in ZipFile");
        }
        else {
            return it->second;
        }
    }

    std::size_t ZipFile::ReadFile(FileBuffer& buffer, const std::string& path) {
        auto file_info = Find(path);
        auto length = file_info.file_info.uncompressed_size;
        buffer.size = length;
        buffer.buffer = new uint8_t[length];
        int32_t err = unzGoToFilePos64(file_.file_pointer, &file_info.file_pos);
        if(err != UNZ_OK) {
            RK_ERROR(File, "Error {} with ZipFile in unzGoToFilePos64", err);
            return 1;
        }
        // TODO : add password support
        //unzOpenCurrentFilePassword(uf, password);
        err = unzOpenCurrentFile(file_.file_pointer);
        if(err < 0) {
            RK_ERROR(File, "Error {} with ZipFile in unzOpenCurrentFile", err);
            return 1;
        }
        err = unzReadCurrentFile(file_.file_pointer, buffer.buffer, length);
        if(err < 0) {
            RK_ERROR(File, "Error {} with ZipFile in unzReadCurrentFile", err);
            return 1;
        }
        err = unzCloseCurrentFile(file_.file_pointer);
        if (err != UNZ_OK) {
            RK_ERROR(File, "Error {} with ZipFile in unzCloseCurrentFile", err);
            return 1;
        }
        return 0;
    }

    std::size_t ZipFile::WriteFile(FileBuffer& buffer, const std::string& path) {
        std::scoped_lock guard{ write_mutex };

        zip_fileinfo zi;
        unsigned long crcFile=0;
        zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
        zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
        zi.dosDate = 0;
        zi.internal_fa = 0;
        zi.external_fa = 0;

        //int zip64 = IsLargeFile(path.c_str());

        // TODO : implement this write file function
        return 0;
    }

    std::future<std::size_t> ZipFile::ReadAllAsync(std::vector<FileBuffer>& buffers) {
        return std::async(std::launch::async, [&](){
            for(auto content : content_) {
                FileBuffer buffer;
                this->ReadFile(buffer, content.first); 
                buffers.push_back(buffer);
            }
            return std::size_t(0);
        });
    }

    std::future<std::size_t> ZipFile::WriteAllAsync(std::vector<FileBuffer>& buffers, std::vector<std::string> paths) {
        return std::async(std::launch::async, [&](){
            for(int i = 0; i < paths.size(); ++i) {
                FileBuffer buffer;
                this->WriteFile(buffer, paths[i]);
                buffers.push_back(buffer);
            }
            return std::size_t(0);
        });
    }

    std::future<std::size_t> ZipFile::ReadAsync(FileBuffer& buffer, const std::string& path) {
        return std::async(std::launch::async, [&](){ return this->ReadFile(buffer, path); });
    }

    std::future<std::size_t> ZipFile::WriteAsync(FileBuffer& buffer, const std::string& path) {
        return std::async(std::launch::async, [&](){ return this->WriteFile(buffer, path); });
    }
}
