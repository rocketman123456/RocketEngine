#pragma once
#include "Memory/MemoryDefine.h"
#include "Utils/Variant.h"
#include "Utils/Delegate.h"
#include "Utils/Hashing.h"
#include "Utils/Timer.h"
#include "Containers/Queue/PriorityQueue.h"
#include "Log/Log.h"

#include <vector>
#include <memory>
#include <cstdint>
#include <string>
#include <cassert>
#include <functional>
#include <unordered_map>

#include <Eigen/Eigen>

namespace Rocket {
    // Forward Declariation
    struct Event;

	using EventType = uint64_t;
    using EventDataPtr = Variant*;
    using EventPtr = std::shared_ptr<Event>;
    using EventDelegate = Delegate<bool(EventPtr&)>;
    using EventFunction = bool (*) (EventPtr&);
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
#ifdef RK_DEBUG
            name_ = event.name_;
#endif
            type_ = event.type_;
            size_ = event.size_;
            variable_ = new Variant[size_];
            std::memcpy(variable_, event.variable_, sizeof(Variant) * size_);
            time_stamp_ = event.time_stamp_;
            time_delay_ = event.time_delay_;
        }
        Event(Event&& event) {
#ifdef RK_DEBUG
            name_ = event.name_;
#endif
            type_ = event.type_;
            size_ = event.size_;
            variable_ = event.variable_;
            time_stamp_ = event.time_stamp_;
            time_delay_ = event.time_delay_;
        }
        virtual ~Event() { if(variable_ != nullptr) delete[] variable_; }

        void Tick(double dt) { time_delay_ -= dt; }
        bool Ready() { return time_delay_ <= 1e-3; }

        EventType GetEventType() const { return type_; }
        int32_t GetInt32(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_int32; }
		uint32_t GetUInt32(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_uint32; }
		float GetFloat(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_float; }
		double GetDouble(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_double; }
		bool GetBool(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_bool; }
		void* GetPointer(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_pointer; }
		string_id GetStringId(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_string_id; }
        const EventDataPtr GetRawData() const { return variable_; }
        std::string ToString();

        Event& operator = (const Event& other) {
#ifdef RK_DEBUG
            name_ = other.name_;
#endif
            type_ = other.type_;
            size_ = other.size_;
            variable_ = new Variant[size_];
            std::memcpy(variable_, other.variable_, sizeof(Variant) * size_);
            time_stamp_ = other.time_stamp_;
            time_delay_ = other.time_delay_;
            return *this;
        }
        Event& operator = (Event&& other) {
#ifdef RK_DEBUG
            name_ = other.name_;
#endif
            type_ = other.type_;
            size_ = other.size_;
            variable_ = other.variable_;
            time_stamp_ = other.time_stamp_;
            time_delay_ = other.time_delay_;
            return *this;
        }
        
        // 0. use different channel to handle different event type
        // 1. update & check delay time, decide use event or not
        // 2. dispatch event to listener
        EventType type_;            //  8 bytes
        uint64_t size_ = 0;         //  8 bytes
        EventDataPtr variable_ = nullptr;   //  4 bytes
		double time_stamp_ = 0.0f;  //  ms  4 bytes
        double time_delay_ = 0.0f;  //  ms  4 bytes
        bool handled_ = false;      //  1 byte
#ifdef RK_DEBUG
        std::string name_;          //  event name
#endif

        static ElapseTimer timer_s;
    };

    struct EventDelayCompare {
        bool operator() (const Rocket::EventPtr& x, const Rocket::EventPtr& y) const {
            return x->time_delay_ < y->time_delay_;
        }
        typedef Rocket::EventPtr first_argument_type;
        typedef Rocket::EventPtr second_argument_type;
        typedef bool result_type;
    };
}
