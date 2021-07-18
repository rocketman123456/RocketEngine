#include "Log/Log.h"

#ifdef RK_CONSOLE_LOG
#include <spdlog/async.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#endif

namespace Rocket {

#ifdef RK_CONSOLE_LOG
    static void SetLevel(LogLevel level, spdlog::logger* logger) {
        switch(level) {
        case LogLevel::TRACE:
            logger->set_level(spdlog::level::trace); break;
        case LogLevel::INFO:
            logger->set_level(spdlog::level::info); break;
        case LogLevel::WARN:
            logger->set_level(spdlog::level::warn); break;
        case LogLevel::ERR:
            logger->set_level(spdlog::level::err); break;
        case LogLevel::CRITICAL:
            logger->set_level(spdlog::level::critical); break;
        }
    }

    IMPLEMENT_LOG_CHANNEL(Core);
    IMPLEMENT_LOG_CHANNEL(App);

    void Log::Init(LogLevel level) {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        INIT_LOG_CHANNEL(Core);
        INIT_LOG_CHANNEL(App);
    }
#else
    void Log::Init(LogLevel level) {}
#endif
} // namespace Rocket