#pragma once
#include <string>
#include <cstdint>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

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

#define alCall(function, ...) ALCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, ...) ALCCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
