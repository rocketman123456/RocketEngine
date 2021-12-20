#include "Log/Log.h"

#ifdef RK_CONSOLE_LOG
#define INIT_LOG_CHANNEL(x) s_##x##_logger_ = spdlog::stdout_color_mt(#x); SetLevel(level, s_##x##_logger_.get());
#define END_LOG_CHANNEL(x) s_##x##_logger_.reset();
#define IMPLEMENT_LOG_CHANNEL(x) std::shared_ptr<spdlog::logger> Rocket::Log::s_##x##_logger_;
#endif

#ifdef RK_CONSOLE_LOG

// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/fmt/fmt.h>

IMPLEMENT_LOG_CHANNEL(Core);
IMPLEMENT_LOG_CHANNEL(Memory);
IMPLEMENT_LOG_CHANNEL(Console);
IMPLEMENT_LOG_CHANNEL(Window);
IMPLEMENT_LOG_CHANNEL(Graphics);
IMPLEMENT_LOG_CHANNEL(Event);
IMPLEMENT_LOG_CHANNEL(File);
IMPLEMENT_LOG_CHANNEL(Audio);
IMPLEMENT_LOG_CHANNEL(App);
#endif

namespace {
    class DefaultLogger {
    public:
        DefaultLogger() { Rocket::Log::Init(); }
        ~DefaultLogger() { Rocket::Log::End(); }
    };

    DefaultLogger _default_logger_ = DefaultLogger();
}

namespace Rocket {
#ifdef RK_CONSOLE_LOG
    static void SetLevel(LogLevel level, spdlog::logger* logger) {
        switch(level) {
        case LogLevel::TRACE:
            logger->set_level(spdlog::level::trace); break;
        case LogLevel::DEBUG:
            logger->set_level(spdlog::level::debug); break;
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

    void Log::Init(LogLevel level) {
        spdlog::set_pattern("%^[%l%$][%T][%n] %v%$");
        INIT_LOG_CHANNEL(Core);
        INIT_LOG_CHANNEL(Memory);
        INIT_LOG_CHANNEL(Console);
        INIT_LOG_CHANNEL(Window);
        INIT_LOG_CHANNEL(Graphics);
        INIT_LOG_CHANNEL(Event);
        INIT_LOG_CHANNEL(File);
        INIT_LOG_CHANNEL(Audio);
        INIT_LOG_CHANNEL(App);
    }

    void Log::End() {
        END_LOG_CHANNEL(Core);
        END_LOG_CHANNEL(Memory);
        END_LOG_CHANNEL(Console);
        END_LOG_CHANNEL(Window);
        END_LOG_CHANNEL(Graphics);
        END_LOG_CHANNEL(Event);
        END_LOG_CHANNEL(File);
        END_LOG_CHANNEL(Audio);
        END_LOG_CHANNEL(App);
    }
#else
    void Log::Init(LogLevel level) {}
    void Log::End() {}
#endif
} // namespace Rocket