#include "FileSystem/ZipFile/ZipFile.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "FileSystem/ZipFile/ZipUtils.h"

#include <exception>
#include <stdexcept>
#include <algorithm>

namespace Rocket {
    ZipFile::ZipFile(const VirtualNodePtr& vnode_) 
        : vnode(vnode_), real_path(GetVirtualPath(vnode)), virtual_path(GetRealPath(vnode)) {}
    ZipFile::ZipFile(const std::string& path_, const std::string& virtual_path_, zip_t* zip_) 
        : real_path(path_), virtual_path(virtual_path_), zip_ptr(zip_) {}
    ZipFile::ZipFile(const std::string& path_, zip_t* zip_) 
        : real_path(path_), zip_ptr(zip_) {}

    void ZipFile::Open(int32_t mode) {
        if(IsOpened()) return;
        this->mode = mode;
        is_read_only = true;
        // TODO : add file write permission check
        if(mode & FileEnum::APPEND) {
            is_read_only = false;
        }
        if(mode & FileEnum::WRITE_BINARY || mode & FileEnum::WRITE_TEXT) {
            is_read_only = false;
        }
        if(mode & FileEnum::TRUNCATE) {
            is_read_only = false;
        }

        zip_file_ptr = OpenZipFile(zip_ptr, real_path);
        if(zip_file_ptr == nullptr) {
            RK_ERROR(File, "Fail to open zip file: {}", real_path);
            // TODO : create not exist file in zip
        } else {
            is_opened = true;
        }

        GetZipFileStatus(zip_ptr, &zip_file_status, real_path);
        RK_TRACE(File, "Name: {}", zip_file_status.name);
        RK_TRACE(File, "\tIndex: {}", zip_file_status.index);
        RK_TRACE(File, "\tCompressed Size: {}", zip_file_status.comp_size);
        RK_TRACE(File, "\tSize: {}", zip_file_status.size);
        RK_TRACE(File, "\tCRC: {}", zip_file_status.crc);
    }

    void ZipFile::Close() {
        if(!IsOpened()) return;
        zip_fclose(zip_file_ptr);
        is_opened = false;
    }

    std::size_t ZipFile::Seek(std::size_t offset, FileEnum::Origin origin) {
        RK_WARN(File, "Unable to Seek in Zip File");
        return 0;
    }

    std::size_t ZipFile::Tell() {
        RK_WARN(File, "Unable to Tell in Zip File");
        return 0;
    }

    std::size_t ZipFile::Read(FileBuffer& data) {
        std::size_t read_size = std::min(Size(), data.size());
        zip_fread(zip_file_ptr, data.data(), data.size());
        return read_size;
    }

    std::size_t ZipFile::Write(const FileBuffer& data) {
        FileBuffer origin_data = {};
        if(mode & FileEnum::APPEND) {
            // TODO : support append to zip file
            FileBuffer append_data = {new FileByte[Size() + data.size()], Size() + data.size()};
            FileBuffer origin_data = {append_data.data(), Size()};
            Read(origin_data);
            std::memcpy((void*)(append_data.data() + Size()), data.data(), data.size());
        } else {
            origin_data = data;
        }

        zip_source_t* source;
        source = zip_source_buffer(zip_ptr, origin_data.data(), origin_data.size(), 0);
        if(source == nullptr) {
            RK_WARN(File, "error adding in zip file: {}", zip_strerror(zip_ptr));
            return 0;
        }
        // ZIP_FL_ENC_UTF_8 : string is UTF-8 encoded
        // ZIP_FL_ENC_CP437 : string is CP437 encoded
        // ZIP_FL_OVERWRITE : zip_file_add: if file with name exists, overwrite (replace) it
        // this will replace file in zip
        auto result = zip_file_add(zip_ptr, zip_file_status.name, source, ZIP_FL_OVERWRITE);
        std::size_t real_size = 0;
        if (result < 0) {
            zip_source_free(source);
            RK_WARN(File, "error adding in zip file: {}", zip_strerror(zip_ptr));
        } else {
            if(mode & FileEnum::APPEND) {
                real_size = data.size();
                delete [] origin_data.data();
            } else {
                real_size = data.size();
            }
        }
        //zip_source_free(source);
        // Update File Info
        GetZipFileStatus(zip_ptr, &zip_file_status, real_path);
        return real_size;
    }
}