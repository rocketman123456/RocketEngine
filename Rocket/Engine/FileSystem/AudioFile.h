#pragma once
// TODO : finish resource cache
//#include "Parser/AudioPraser.h"
#include "FileSystem/OsFile.h"
#include "Utils/AudioChecker.h"

#include <memory>
#include <cstdint>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Rocket {
    class AudioFile : implements OsFile {
    public:
        AudioFile() = default;
        virtual ~AudioFile() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;

        virtual std::size_t Read(FileBuffer& buffer, std::size_t length) final;
        virtual std::size_t ReadAll(FileBuffer& buffer) final;
        virtual std::size_t Write(FileBuffer& buffer, std::size_t length) final;
        virtual void Seek(std::size_t position) final;
        virtual void SeekToEnd(void) final;
        virtual void Skip(std::size_t bytes) final;
        //virtual std::size_t Tell(void) const;

    private:
        void CheckSndFileError();
    };

    using AudioFilePtr = std::unique_ptr<AudioFile>;
}