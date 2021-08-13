#pragma once
#include "Parser/AudioPraser.h"
#include "FileSystem/OsFile.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <memory>
#include <cstdint>
#include <string>

bool CheckALErrors(const std::string& filename, const std::uint_fast32_t line);
bool CheckALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

template<typename alFunction, typename... Params>
auto ALCallImpl(
    const char* filename,
    const std::uint_fast32_t line,
    alFunction function,
    Params... params)
    ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
    auto ret = function(std::forward<Params>(params)...);
    CheckALErrors(filename, line);
    return ret;
}

template<typename alFunction, typename... Params>
auto ALCallImpl(
    const char* filename,
    const std::uint_fast32_t line,
    alFunction function,
    Params... params)
    ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return CheckALErrors(filename, line);
}

template<typename alcFunction, typename... Params>
auto ALCCallImpl(
    const char* filename, 
    const std::uint_fast32_t line, 
    alcFunction function, 
    ALCdevice* device, 
    Params... params)
    ->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool>
{
    function(std::forward<Params>(params)...);
    return CheckALCErrors(filename,line,device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
auto ALCCallImpl(
    const char* filename,
    const std::uint_fast32_t line,
    alcFunction function,
    ReturnType& returnValue,
    ALCdevice* device, 
    Params... params)
    ->typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool>
{
    returnValue = function(std::forward<Params>(params)...);
    return CheckALCErrors(filename,line,device);
}

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

        virtual void Seek(std::size_t position);
        virtual void SeekToEnd(void);
        virtual void Skip(std::size_t bytes);
        virtual std::size_t Tell(void) const;
    };

    using AudioFilePtr = std::unique_ptr<AudioFile>;
}