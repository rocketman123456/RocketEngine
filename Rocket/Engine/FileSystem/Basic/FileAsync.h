#pragma once
#include "FileSystem/Basic/File.h"

#include <gsl/gsl>
#include <future>

namespace Rocket {
    _Interface_ FileAsync : _inherits_ File {
    public:
        virtual std::future<std::size_t> ReadAsync(gsl::span<gsl::byte>& data) = 0;
        virtual std::future<std::size_t> WriteAsync(gsl::span<gsl::byte> data) = 0;
    };
}
