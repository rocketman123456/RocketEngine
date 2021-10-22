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
        float* data_ = stbi_loadf_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch, 0);
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
        float* data_ = stbi_loadf_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch, de_ch);
        RK_INFO(File, "Load Image {}:{},{},{}", full_path_, x, y, ch);
        data.clear();
        data.resize(x * y);
        // Copy Data
        for(int i = 0; i < x; ++i) {
            for(int j = 0; j < y; ++j) {
                if(ch == 3) {
                    int index = (i + x * j) * ch;
                    data[i + x * j] = Eigen::Vector4f(data_[index], data_[index+1], data_[index+2], 1.0f);
                } else if(ch == 4) {
                    int index = (i + x * j) * ch;
                    data[i + x * j] = Eigen::Vector4f(data_[index], data_[index+1], data_[index+2], data_[index+3]);
                } else if(ch == 1) {
                    int index = (i + x * j) * ch;
                    data[i + x * j] = Eigen::Vector4f(data_[index], data_[index], data_[index], 1.0f);
                }
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
        float* data_ = stbi_loadf_from_memory((const unsigned char *)buffer.buffer, buffer.size, &x, &y, &ch, de_ch);
        RK_INFO(File, "Load Image {}:{},{},{}", full_path_, x, y, ch);
        data.clear();
        data.resize(x * y);
        // Copy Data
        for(int i = 0; i < x; ++i) {
            for(int j = 0; j < y; ++j) {
                if(ch == 3) {
                    int index = (i + x * j) * ch;
                    data[i + x * j] = Eigen::Vector3f(data_[index], data_[index+1], data_[index+2]);
                } else if(ch == 4) {
                    int index = (i + x * j) * ch;
                    data[i + x * j] = Eigen::Vector3f(data_[index], data_[index+1], data_[index+2]);
                } else if(ch == 1) {
                    int index = (i + x * j) * ch;
                    data[i + x * j] = Eigen::Vector3f(data_[index], data_[index], data_[index]);
                }
            }
        }
        stbi_image_free(data_);
        return { x, y };
    }
}
