#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

#include "Log/Log.h"

namespace Rocket {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult {
		std::string name;
		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsed_time;
		std::thread::id thread_id;
	};

	struct InstrumentationSession {
		std::string name;
	};

	class Instrumentor {
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json");
		void EndSession();
		void WriteProfile(const ProfileResult& result);

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor() : current_session_(nullptr) {}
		~Instrumentor() { EndSession(); }
		void WriteHeader();
		void WriteFooter();
		void InternalEndSession();
	private:
		std::mutex mutex_;
		InstrumentationSession* current_session_;
		std::ofstream output_stream_;
	};

	class InstrumentationTimer {
	public:
		InstrumentationTimer(const char* name);
		~InstrumentationTimer();
		void Stop();
	private:
		const char* name_;
		std::chrono::time_point<std::chrono::steady_clock> start_timepoint_;
		bool stopped_ = false;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult {
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K]) {
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N) {
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#if defined(RK_PROFILE_SETTINGS)
	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define RK_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define RK_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define RK_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define RK_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define RK_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define RK_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define RK_FUNC_SIG __func__
	#else
		#define RK_FUNC_SIG "RK_FUNC_SIG unknown!"
	#endif

	#define RK_PROFILE_BEGIN_SESSION(name, filepath) ::Rocket::Instrumentor::Get().BeginSession(name, filepath)
	#define RK_PROFILE_END_SESSION() ::Rocket::Instrumentor::Get().EndSession()
	#define RK_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Rocket::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::Rocket::InstrumentationTimer timer##line(fixedName##line.Data)
	#define RK_PROFILE_SCOPE_LINE(name, line) RK_PROFILE_SCOPE_LINE2(name, line)
	#define RK_PROFILE_SCOPE(name) RK_PROFILE_SCOPE_LINE(name, __LINE__)
	#define RK_PROFILE_FUNCTION() RK_PROFILE_SCOPE(RK_FUNC_SIG)
#else
	#define RK_PROFILE_BEGIN_SESSION(name, filepath)
	#define RK_PROFILE_END_SESSION()
	#define RK_PROFILE_SCOPE(name)
	#define RK_PROFILE_FUNCTION()
#endif