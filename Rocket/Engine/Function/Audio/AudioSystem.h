#pragma once

#include <miniaudio.h>

#include <cstdint>
#include <list>
#include <unordered_map>

namespace Rocket
{
    class AudioSystem
    {
    public:
        AudioSystem();
        ~AudioSystem();

        void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);

    private:
        std::unordered_map<uint32_t, uint32_t> m_audio_map;
    };
} // namespace Rocket
