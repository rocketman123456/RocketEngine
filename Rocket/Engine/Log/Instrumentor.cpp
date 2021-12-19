#include "Log/Instrumentor.h"

namespace Rocket {
    void Instrumentor::BeginSession(const std::string& name, const std::string& filepath) {
        std::lock_guard lock(mutex_);
        if (current_session_) {
            // If there is already a current session, then close it before beginning new one.
            // Subsequent profiling output meant for the original session will end up in the
            // newly opened session instead.  That's better than having badly formatted
            // profiling output.
            if (Log::GetCoreLogger()) { // Edge case: BeginSession() might be before Log::Init()
                RK_ERROR(Core, "Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, current_session_->name);
            }
            InternalEndSession();
        }
        output_stream_.open(filepath);

        if (output_stream_.is_open()) {
            current_session_ = new InstrumentationSession({name});
            WriteHeader();
        } else {
            if (Log::GetCoreLogger()) { // Edge case: BeginSession() might be before Log::Init()
                RK_ERROR(Core, "Instrumentor could not open results file '{0}'.", filepath);
            }
        }
    }

    void Instrumentor::EndSession() {
        std::lock_guard lock(mutex_);
        InternalEndSession();
    }

    void Instrumentor::WriteProfile(const ProfileResult& result) {
        std::stringstream json;

        json << std::setprecision(3) << std::fixed;
        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << (result.elapsed_time.count()) << ',';
        json << "\"name\":\"" << result.name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":" << result.thread_id << ",";
        json << "\"ts\":" << result.start.count();
        json << "}";

        std::lock_guard lock(mutex_);
        if (current_session_) {
            output_stream_ << json.str();
            output_stream_.flush();
        }
    }

    void Instrumentor::WriteHeader() {
        output_stream_ << "{\"otherData\": {},\"traceEvents\":[{}";
        output_stream_.flush();
    }

    void Instrumentor::WriteFooter() {
        output_stream_ << "]}";
        output_stream_.flush();
    }

    // Note: you must already own lock on m_Mutex before
    // calling InternalEndSession()
    void Instrumentor::InternalEndSession() {
        if (current_session_) {
            WriteFooter();
            output_stream_.close();
            delete current_session_;
            current_session_ = nullptr;
        }
    }

    InstrumentationTimer::InstrumentationTimer(const char* name) : name_(name) {
        start_timepoint_ = std::chrono::steady_clock::now();
    }

    InstrumentationTimer::~InstrumentationTimer() {
        if (!stopped_)
            Stop();
    }

    void InstrumentationTimer::Stop() {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = FloatingPointMicroseconds{ start_timepoint_.time_since_epoch() };
        auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint_).time_since_epoch();

        Instrumentor::Get().WriteProfile({ name_, highResStart, elapsedTime, std::this_thread::get_id() });

        stopped_ = true;
    }
}