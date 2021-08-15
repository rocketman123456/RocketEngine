#pragma once
// TODO : finish resource cache
#include "Parser/AudioPraser.h"
#include "FileSystem/OsFile.h"
#include "Utils/AudioChecker.h"

#include <memory>
#include <cstdint>
#include <string>

namespace Rocket {
    using AudioBuffer = FileBuffer;

    class AudioFile : implements OsFile {
    public:
        AudioFile() = default;
        virtual ~AudioFile() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;

        virtual std::size_t Read(AudioBuffer& buffer, std::size_t length) final;
        virtual std::size_t ReadAll(AudioBuffer& buffer) final;
        virtual std::size_t Write(AudioBuffer& buffer, std::size_t length) final;
        virtual std::size_t Write(AudioBuffer& buffer) final;
        virtual void Seek(std::size_t position) final;
        virtual void SeekToEnd(void) final;
        virtual void Skip(std::size_t bytes) final;

    private:
        void CheckSndFileError();
    };

    using AudioFilePtr = std::unique_ptr<AudioFile>;
}