#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace Rocket
{
    enum class LogLevel {
        TRACE = 0,
        INFO,
        WARN,
        ERR,
        CRITICAL,
    };

    class Log {
    public:
        Log() {}
        ~Log() {}

        static void Init(LogLevel level = LogLevel::TRACE);

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_core_logger_; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_client_logger_; }
        inline static std::shared_ptr<spdlog::logger>& GetEventLogger() { return s_event_logger_; }
        inline static std::shared_ptr<spdlog::logger>& GetGraphicsLogger() { return s_graphics_logger_; }

    private:
        static std::shared_ptr<spdlog::logger> s_core_logger_;
        static std::shared_ptr<spdlog::logger> s_client_logger_;
        static std::shared_ptr<spdlog::logger> s_event_logger_;
        static std::shared_ptr<spdlog::logger> s_graphics_logger_;
    };
} // namespace Rocket

#ifdef RK_DEBUG

#define RK_CORE_CRITICAL(...)   ::Rocket::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define RK_CORE_ERROR(...)      ::Rocket::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RK_CORE_WARN(...)       ::Rocket::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RK_CORE_INFO(...)       ::Rocket::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RK_CORE_TRACE(...)      ::Rocket::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define RK_EVENT_CRITICAL(...)  ::Rocket::Log::GetEventLogger()->critical(__VA_ARGS__)
#define RK_EVENT_ERROR(...)     ::Rocket::Log::GetEventLogger()->error(__VA_ARGS__)
#define RK_EVENT_WARN(...)      ::Rocket::Log::GetEventLogger()->warn(__VA_ARGS__)
#define RK_EVENT_INFO(...)      ::Rocket::Log::GetEventLogger()->info(__VA_ARGS__)
#define RK_EVENT_TRACE(...)     ::Rocket::Log::GetEventLogger()->trace(__VA_ARGS__)

#define RK_GRAPHICS_CRITICAL(...)   ::Rocket::Log::GetGraphicsLogger()->critical(__VA_ARGS__)
#define RK_GRAPHICS_ERROR(...)      ::Rocket::Log::GetGraphicsLogger()->error(__VA_ARGS__)
#define RK_GRAPHICS_WARN(...)       ::Rocket::Log::GetGraphicsLogger()->warn(__VA_ARGS__)
#define RK_GRAPHICS_INFO(...)       ::Rocket::Log::GetGraphicsLogger()->info(__VA_ARGS__)
#define RK_GRAPHICS_TRACE(...)      ::Rocket::Log::GetGraphicsLogger()->trace(__VA_ARGS__)

#define RK_CRITICAL(...)    ::Rocket::Log::GetClientLogger()->critical(__VA_ARGS__)
#define RK_ERROR(...)       ::Rocket::Log::GetClientLogger()->error(__VA_ARGS__)
#define RK_WARN(...)        ::Rocket::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RK_INFO(...)        ::Rocket::Log::GetClientLogger()->info(__VA_ARGS__)
#define RK_TRACE(...)       ::Rocket::Log::GetClientLogger()->trace(__VA_ARGS__)

#else

#define RK_CORE_CRITICAL(...)
#define RK_CORE_ERROR(...)
#define RK_CORE_WARN(...)
#define RK_CORE_INFO(...)
#define RK_CORE_TRACE(...)

#define RK_EVENT_CRITICAL(...)
#define RK_EVENT_ERROR(...)
#define RK_EVENT_WARN(...)
#define RK_EVENT_INFO(...)
#define RK_EVENT_TRACE(...)

#define RK_GRAPHICS_CRITICAL(...)
#define RK_GRAPHICS_ERROR(...)
#define RK_GRAPHICS_WARN(...)
#define RK_GRAPHICS_INFO(...)
#define RK_GRAPHICS_TRACE(...)

#define RK_CRITICAL(...)
#define RK_ERROR(...)
#define RK_WARN(...)
#define RK_INFO(...)
#define RK_TRACE(...)

#endif