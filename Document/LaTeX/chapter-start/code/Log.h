#pragma once
#ifdef RK_CONSOLE_LOG
#include <memory>
#include <unordered_map>
#include <spdlog/spdlog.h>

#define INIT_LOG_CHANNEL(x) s_##x##_logger_ = spdlog::stdout_color_mt(#x);\
    SetLevel(level, s_##x##_logger_.get());
#define END_LOG_CHANNEL(x) s_##x##_logger_.reset();
#define DECLARE_LOG_CHANNEL(x) \
    public:\
        inline static spdlog::logger* Get##x##Logger() { \
            return s_##x##_logger_.get(); } \
    private:\
        static std::shared_ptr<spdlog::logger> s_##x##_logger_;
#define IMPLEMENT_LOG_CHANNEL(x) std::shared_ptr<spdlog::logger>\
    Rocket::Log::s_##x##_logger_;
#endif

namespace Rocket {
    enum class LogLevel {
        TRACE = 0, INFO, WARN, ERR, CRITICAL,
    };

    class Log {
    public:
        static void Init(LogLevel level = LogLevel::TRACE);
        static void End();
#ifdef RK_CONSOLE_LOG
        // ÉùÃ÷LogÊä³öÆµµÀ
        DECLARE_LOG_CHANNEL(Core);
        DECLARE_LOG_CHANNEL(Window);
        DECLARE_LOG_CHANNEL(Render);
        DECLARE_LOG_CHANNEL(Event);
        DECLARE_LOG_CHANNEL(File);
        DECLARE_LOG_CHANNEL(Audio);
        DECLARE_LOG_CHANNEL(App);
#endif
    };
} // namespace Rocket

#ifdef RK_CONSOLE_LOG

#define RK_CRITICAL(x, ...) \
    do{::Rocket::Log::Get##x##Logger()->critical(__VA_ARGS__);}while(0);
#define RK_ERROR(x, ...) \
    do{::Rocket::Log::Get##x##Logger()->error(__VA_ARGS__);}while(0);
#define RK_WARN(x, ...) \
    do{::Rocket::Log::Get##x##Logger()->warn(__VA_ARGS__);}while(0);
#define RK_INFO(x, ...) \ 
    do{::Rocket::Log::Get##x##Logger()->info(__VA_ARGS__);}while(0);
#define RK_TRACE(x, ...) \ 
    do{::Rocket::Log::Get##x##Logger()->trace(__VA_ARGS__);}while(0);

#else

#define RK_CRITICAL(x, ...)
#define RK_ERROR(x, ...)
#define RK_WARN(x, ...)
#define RK_INFO(x, ...)
#define RK_TRACE(x, ...)

#endif