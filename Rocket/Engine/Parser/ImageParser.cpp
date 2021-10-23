#include "Parser/ImageParser.h"
#include "Memory/MemoryCheck.h"
#include "FileSystem/FileSystem.h"
#include "Log/Log.h"

// #define STB_DEFINE
// #include <stb.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// #define STB_IMAGE_RESIZE_IMPLEMENTATION
// #include <stb_image_resize.h>
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb_image_write.h>

namespace Rocket {
    std::pair<int32_t, int32_t> ImageParser::Parse() {
        // Just For Test
        FileBuffer buffer;
        FileSystem::LoadSync(full_path_, FileOperateMode::READ_BINARY, buffer);
        int32_t x, y, ch;
#ifdef RK_DEBUG
        int result = stbi_info_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch);
        if(result == 0) {
            RK_ERROR(File, "Unsupported File");
            return {-1, -1};
        }
#endif
        unsigned char* data_ = stbi_load_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch, 0);
        if(data_ == nullptr) {
            RK_ERROR(File, "Error: {}", stbi_failure_reason());
            return {-1, -1};
        }
        RK_INFO(File, "Load Image {}:{},{},{}", full_path_, x, y, ch);
        // Do Nothing
        stbi_image_free(data_);
        return { x, y };
    }

    std::pair<int32_t, int32_t> ImageParser::Parse(std::vector<Eigen::Vector4f>& data) {
        FileBuffer buffer;
        FileSystem::LoadSync(full_path_, FileOperateMode::READ_BINARY, buffer);
        int32_t x, y, ch;
        int32_t de_ch = 4;
#ifdef RK_DEBUG
        int result = stbi_info_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch);
        if(result == 0) {
            RK_ERROR(File, "Unsupported File");
            return {-1, -1};
        }
#endif
        float* data_ = stbi_loadf_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch, de_ch);
        if(data_ == nullptr) {
            RK_ERROR(File, "Error: {}", stbi_failure_reason());
            return {-1, -1};
        }
        
        RK_INFO(File, "Load Image {}:{},{},{}", full_path_, x, y, ch);
        data.clear();
        data.resize(x * y);
        // Copy Data
        for(int i = 0; i < x; ++i) {
            for(int j = 0; j < y; ++j) {
                int index = (i + x * j) * de_ch;
                data[i + x * j] = Eigen::Vector4f(data_[index], data_[index+1], data_[index+2], data_[index+3]) * 255.0f;
                // if(de_ch == 1) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector4f(data_[index], data_[index], data_[index], 1.0f);
                // } else if(de_ch == 2) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector4f(data_[index], data_[index], data_[index], data_[index+1]);
                // } if(de_ch == 3) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector4f(data_[index], data_[index+1], data_[index+2], 1.0f);
                // } else if(de_ch == 4) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector4f(data_[index], data_[index+1], data_[index+2], data_[index+3]);
                // }
            }
        }
        stbi_image_free(data_);
        return { x, y };
    }

    std::pair<int32_t, int32_t> ImageParser::Parse(std::vector<Eigen::Vector3f>& data) {
        FileBuffer buffer;
        FileSystem::LoadSync(full_path_, FileOperateMode::READ_BINARY, buffer);
        int32_t x, y, ch;
        int32_t de_ch = 3;
#ifdef RK_DEBUG
        int result = stbi_info_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch);
        if(result == 0) {
            RK_ERROR(File, "Unsupported File");
            return {-1, -1};
        }
#endif
        float* data_ = stbi_loadf_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch, de_ch);
        if(data_ == nullptr) {
            RK_ERROR(File, "Error: {}", stbi_failure_reason());
            return {-1, -1};
        }
        
        RK_INFO(File, "Load Image {}:{},{},{}", full_path_, x, y, ch);
        data.clear();
        data.resize(x * y);
        // Copy Data
        for(int i = 0; i < x; ++i) {
            for(int j = 0; j < y; ++j) {
                int index = (i + x * j) * de_ch;
                data[i + x * j] = Eigen::Vector3f(data_[index], data_[index+1], data_[index+2]) * 255.0f;
                // if(de_ch == 1) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector3f(data_[index], data_[index], data_[index]);
                // } else if(de_ch == 2) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector3f(data_[index], data_[index], data_[index]);
                // } else if(de_ch == 3) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector3f(data_[index], data_[index+1], data_[index+2]);
                // } else if(de_ch == 4) {
                //     int index = (i + x * j) * de_ch;
                //     data[i + x * j] = Eigen::Vector3f(data_[index], data_[index+1], data_[index+2]);
                // }
            }
        }
        stbi_image_free(data_);
        return { x, y };
    }
}
