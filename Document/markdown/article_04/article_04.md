# 4. 日志输出<br>

## 4.1. 为什么需要日志<br>

## 4.2. 对spdlog进行封装<br>
`Log.h`
```
#pragma once
#ifdef RK_CONSOLE_LOG
#include <memory>
#include <unordered_map>
#include <spdlog/spdlog.h>

#define INIT_LOG_CHANNEL(x) s_##x##_logger_ = spdlog::stdout_color_mt(#x); SetLevel(level, s_##x##_logger_.get());
#define DECLARE_LOG_CHANNEL(x) \
    public:\
        inline static spdlog::logger* Get##x##Logger() { return s_##x##_logger_.get(); } \
    private:\
        static std::shared_ptr<spdlog::logger> s_##x##_logger_;
#define IMPLEMENT_LOG_CHANNEL(x) std::shared_ptr<spdlog::logger> Rocket::Log::s_##x##_logger_;
#endif

namespace Rocket {
    enum class LogLevel {
        TRACE = 0, INFO, WARN, ERR, CRITICAL,
    };

    class Log {
    public:
        static void Init(LogLevel level = LogLevel::TRACE);
#ifdef RK_CONSOLE_LOG
        // 声明Log输出频道
        DECLARE_LOG_CHANNEL(Core);
        DECLARE_LOG_CHANNEL(Event);
        DECLARE_LOG_CHANNEL(File);
        DECLARE_LOG_CHANNEL(App);
#endif
    };
}

#ifdef RK_CONSOLE_LOG

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
```
`Log.cpp`
```
#include "Log/Log.h"

#ifdef RK_CONSOLE_LOG
#include <spdlog/async.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/fmt/fmt.h>
#include <fmt/ostream.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

IMPLEMENT_LOG_CHANNEL(Core);
IMPLEMENT_LOG_CHANNEL(Event);
IMPLEMENT_LOG_CHANNEL(File);
IMPLEMENT_LOG_CHANNEL(App);
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

    void Log::Init(LogLevel level) {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        INIT_LOG_CHANNEL(Core);
        INIT_LOG_CHANNEL(Event);
        INIT_LOG_CHANNEL(File);
        INIT_LOG_CHANNEL(App);
    }
#else
    void Log::Init(LogLevel level) {}
#endif
}
```
## 4.3. 日志输出测试<br>
`log_test.cpp`
```
#include "Log/Log.h"

#include <iostream>
#include <string>

int main() {
    Rocket::Log::Init();

    RK_INFO(Core, "Hello Rocket Engine {}", std::string("Now"));
    RK_TRACE(App, "Hello Rocket App");
    RK_INFO(App, "Hello Rocket App");
    RK_WARN(App, "Hello Rocket App");
    RK_ERROR(App, "Hello Rocket App");
    RK_CRITICAL(App, "Hello Rocket App");

    return 0;
}
```