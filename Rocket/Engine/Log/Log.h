#pragma once
#include <memory>
#include <spdlog/spdlog.h>

#define DECLARE_LOG_CHANNEL(x) \
    public:\
        inline static spdlog::logger* Get##x##Logger() { return s_##x##_logger_.get(); } \
    private:\
        static std::shared_ptr<spdlog::logger> s_##x##_logger_;
#define IMPLEMENT_LOG_CHANNEL(x) std::shared_ptr<spdlog::logger> Log::s_##x##_logger_;
#define INIT_LOG_CHANNEL(x) s_##x##_logger_ = spdlog::stdout_color_mt(#x); SetLevel(level, s_##x##_logger_.get());

namespace Rocket {
    enum class LogLevel {
        TRACE = 0, INFO, WARN, ERROR, CRITICAL,
    };

    class Log {
    public:
        static void Init(LogLevel level = LogLevel::TRACE);

        DECLARE_LOG_CHANNEL(Core);
        DECLARE_LOG_CHANNEL(App);
    };
} // namespace Rocket

#ifdef RK_DEBUG

#define RK_CRITICAL(x, ...)     ::Rocket::Log::Get##x##Logger()->critical(__VA_ARGS__)
#define RK_ERROR(x, ...)        ::Rocket::Log::Get##x##Logger()->error(__VA_ARGS__)
#define RK_WARN(x, ...)         ::Rocket::Log::Get##x##Logger()->warn(__VA_ARGS__)
#define RK_INFO(x, ...)         ::Rocket::Log::Get##x##Logger()->info(__VA_ARGS__)
#define RK_TRACE(x, ...)        ::Rocket::Log::Get##x##Logger()->trace(__VA_ARGS__)

#else

#define RK_CRITICAL(x, ...)
#define RK_ERROR(x, ...)
#define RK_WARN(x, ...)
#define RK_INFO(x, ...)
#define RK_TRACE(x, ...)

#endif