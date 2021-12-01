#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Utils/Variant.h"
#include "Utils/Delegate.h"
#include "Utils/Hashing.h"
#include "Utils/Timer.h"
#include "Log/Log.h"

#include <memory>
#include <cstdint>
#include <string>
#include <cassert>
#include <mutex>
#include <chrono>
#include <condition_variable>

namespace Rocket {
    // Forward Declariation
    struct Event;

	using EventType = uint64_t;
    using EventDataPtr = Variant*;
    //typedef bool(*EventFunction)(EventPtr&);

    // TODO : reflect event data
    // TODO : add event time stamp
    struct alignas(16) Event {
    public:
        explicit Event(const std::string& name);
        explicit Event(const std::string& name, EventType type);
		explicit Event(const std::string& name, EventDataPtr ptr, uint64_t size);
        explicit Event(const std::string& name, EventType type, EventDataPtr ptr, uint64_t size);

        Event(const Event& event) {
            name = event.name;
            type = event.type;
            size = event.size;
            variable = new Variant[size];
            std::memcpy(variable, event.variable, sizeof(Variant) * size);
            time_stamp = event.time_stamp;
            time_delay = event.time_delay;
        }
        Event(Event&& event) {
            name = event.name;
            type = event.type;
            size = event.size;
            variable = event.variable;
            time_stamp = event.time_stamp;
            time_delay = event.time_delay;
        }
        virtual ~Event() { if(variable != nullptr) delete[] variable; }

        void Tick(double dt) { time_delay -= dt; }
        bool Ready() { return time_delay <= 1e-3; }

        EventType GetEventType() const { return type; }
        int32_t GetInt32(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_int32; }
		uint32_t GetUInt32(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_uint32; }
		float GetFloat(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_float; }
		double GetDouble(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_double; }
		bool GetBool(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_bool; }
		void* GetPointer(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_pointer; }
		string_id GetStringId(uint64_t index) { assert(index < size && "event index error"); return variable[index].as_string_id; }
        const EventDataPtr GetRawData() const { return variable; }
        std::string ToString();

        Event& operator = (const Event& other) {
            if(size > 0 && variable != nullptr)
                delete variable;
            name = other.name;
            type = other.type;
            size = other.size;
            if(other.size > 0 && other.variable != nullptr) {
                variable = new Variant[size];
                std::memcpy(variable, other.variable, sizeof(Variant) * size);
            }
            time_stamp = other.time_stamp;
            time_delay = other.time_delay;
            return *this;
        }

        Event& operator = (Event&& other) {
            name = other.name;
            type = other.type;
            size = other.size;
            variable = other.variable;
            time_stamp = other.time_stamp;
            time_delay = other.time_delay;
            return *this;
        }
        
        // 0. use different channel to handle different event type
        // 1. update & check delay time
        // 2. dispatch event to listener
        EventType type;            //  8 bytes
        uint64_t size = 0;         //  8 bytes, Event Data Size
        EventDataPtr variable = nullptr;   //  4 bytes
		double time_stamp = 0.0f;  //  ms  4 bytes
        double time_delay = 0.0f;  //  ms  4 bytes
        bool handled = false;      //  1 byte
        std::string name;          //  event name

        static ElapseTimer timer_s;
    };

    using EventPtr = std::shared_ptr<Event>;
    using EventDelegate = Delegate<bool(EventPtr&)>;
    using EventFunction = bool (*) (EventPtr&);

    struct EventDelayCompare {
        bool operator() (const Rocket::EventPtr& x, const Rocket::EventPtr& y) const {
            return x->time_delay < y->time_delay;
        }
        typedef Rocket::EventPtr first_argument_type;
        typedef Rocket::EventPtr second_argument_type;
        typedef bool result_type;
    };

    class ManualEvent {
    public:
        explicit ManualEvent(bool signaled = false)
        : m_signaled(signaled) {}

        void signal() {
            {
                std::unique_lock lock(m_mutex);
                m_signaled = true;
            }
            m_cv.notify_all();
        }

        void wait() {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [&]() { return m_signaled != false; });
        }

        template<typename Rep, typename Period>
        bool wait_for(const std::chrono::duration<Rep, Period>& t) {
            std::unique_lock lock(m_mutex);
            bool result = m_cv.wait_for(lock, t, [&]() { return m_signaled != false; });
            return result;
        }

        template<typename Clock, typename Duration>
        bool wait_until(const std::chrono::time_point<Clock, Duration>& t) {
            std::unique_lock lock(m_mutex);
            bool result = m_cv.wait_until(lock, t, [&]() { return m_signaled != false; });
            return result;
        }

        void reset() {
            std::unique_lock lock(m_mutex);
            m_signaled = false;
        }

    private:
        bool m_signaled = false;
        std::mutex m_mutex;
        std::condition_variable m_cv;
    };

    class AutoEvent {
    public:
        explicit AutoEvent(bool signaled = false)
        : m_signaled(signaled) {}

        void signal() {
            {
                std::unique_lock lock(m_mutex);
                m_signaled = true;
            }
            m_cv.notify_one();
        }

        void wait() {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [&]() { return m_signaled != false; });
            m_signaled = false;
        }

        template<typename Rep, typename Period>
        bool wait_for(const std::chrono::duration<Rep, Period>& t) {
            std::unique_lock lock(m_mutex);
            bool result = m_cv.wait_for(lock, t, [&]() { return m_signaled != false; });
            if(result) m_signaled = false;
            return result;
        }

        template<typename Clock, typename Duration>
        bool wait_until(const std::chrono::time_point<Clock, Duration>& t) {
            std::unique_lock lock(m_mutex);
            bool result = m_cv.wait_until(lock, t, [&]() { return m_signaled != false; });
            if(result) m_signaled = false;
            return result;
        }

    private:
        bool m_signaled = false;
        std::mutex m_mutex;
        std::condition_variable m_cv;
    };
}
