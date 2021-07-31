#pragma once
#include "Utils/Variant.h"
#include "Log/Log.h"

#include <vector>
#include <memory>
#include <cstdint>
#include <string>
#include <cassert>

namespace Rocket {
    // Forward Declariation
    struct Event;

    using EventVarVec = std::vector<Variant>;
	using EventVarPtr = std::unique_ptr<Variant>;
	using EventType = uint64_t;
    using EventUPtr = std::unique_ptr<Event>;
    using EventSPtr = std::shared_ptr<Event>;

    // Event Variable [0] = event type (hash string id)
    // Event Variable [1 - n] = event data
    // TODO : reflect event data
    struct Event {
        explicit Event(EventVarVec&& var, const std::string& name = "event") 
            : variable_(var), name_(name) {}
		virtual ~Event() = default;

        void Tick(double dt) { time_delay_ -= dt; }
        bool Ready() { return time_delay_ <= 1e-3; }

        [[nodiscard]] EventType GetEventType() const { return variable_[0].as_string_id; }
        [[nodiscard]] int32_t GetInt32(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_int32; }
		[[nodiscard]] uint32_t GetUInt32(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_uint32; }
		[[nodiscard]] float GetFloat(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_float; }
		[[nodiscard]] double GetDouble(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_double; }
		[[nodiscard]] bool GetBool(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_bool; }
		[[nodiscard]] void* GetPointer(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_pointer; }
		[[nodiscard]] string_id GetStringId(uint64_t index) { assert(index < variable_.size() && "event index error"); return variable_[index].as_string_id; }
        [[nodiscard]] virtual std::string ToString();

        bool handled_ = false;
        // 0. use different channel to handle different event type
        // 1. update & check delay time, decide use event or not
        // 2. dispatch event to listener
        // TODO : add timer to init event
		double time_stamp_ = 0.0f;  //  ms
        double time_delay_ = 0.0f;  //  ms
        EventVarVec variable_;      //  store event data
        std::string name_;          //  event name
    };
}
