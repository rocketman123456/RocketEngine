#pragma once
#include "Core/Core.h"
#include "Utils/Timer.h"
#include "Utils/Variant.h"
#include "Utils/HashFunction.h"
//#include "Reflection/Reflection.h"

#include <utility>
#include <optional>
#include <functional>
#include <sstream>
#include <vector>
#include <memory>

namespace Rocket {
	using EventVarVec = std::vector<Variant>;
	// TODO: Replace with unique_ptr
	using EventVarPtr = std::shared_ptr<Variant>;
	using EventType = uint64_t;

	extern ElapseTimer* g_EventTimer;

	class Event {
	public:
		explicit Event(const EventVarVec& var) : variable_(var), name_("event") { 
            time_stamp_ = g_EventTimer->GetExactTime(); 
        }
		explicit Event(const EventVarVec& var, const std::string& name) : variable_(var), name_(name) { 
            time_stamp_ = g_EventTimer->GetExactTime(); 
        }
		virtual ~Event() = default;

		[[nodiscard]] virtual EventType GetEventType() const { return variable_[0].asStringId; }
		[[nodiscard]] virtual const std::string& ToString() const { return name_; }

		[[nodiscard]] int32_t GetInt32(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asInt32; }
		[[nodiscard]] uint32_t GetUInt32(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asUInt32; }
		[[nodiscard]] float GetFloat(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asFloat; }
		[[nodiscard]] double GetDouble(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asDouble; }
		[[nodiscard]] bool GetBool(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asBool; }
		[[nodiscard]] void* GetPointer(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asPointer; }
		[[nodiscard]] string_id GetStringId(uint32_t index) { RK_CORE_ASSERT(index < variable_.size(), "event index error"); return variable_[index].asStringId; }

		bool handled_ = false;
		double time_stamp_ = 0.0f;
		std::vector<Variant> variable_;     // TODO: use memory manager in std::vector
        std::string name_;

		//REFLECT()
	};

	// TODO: Replace with unique_ptr
	using EventPtr = std::shared_ptr<Event>;

	inline std::ostream& operator << (std::ostream& os, const Event &e) {
		os << e.ToString();
		for (uint32_t i = 0; i < e.variable_.size(); ++i) {
			switch (e.variable_[i].type) {
			case Variant::TYPE_INT32:
				os << "[" << e.variable_[i].asInt32 << "]"; break;
			case Variant::TYPE_UINT32:
				os << "[" << e.variable_[i].asUInt32 << "]"; break;
			case Variant::TYPE_FLOAT:
				os << "[" << e.variable_[i].asFloat << "]"; break;
			case Variant::TYPE_DOUBLE:
				os << "[" << e.variable_[i].asDouble << "]"; break;
			case Variant::TYPE_POINTER:
				os << "[" << e.variable_[i].asPointer << "]"; break;
			case Variant::TYPE_STRING_ID:
				os << "[" << e.variable_[i].asStringId << "]"; break;
			default:
				RK_EVENT_ERROR("Unknow Event Data Type"); break;
			}
		}
		return os;
	}

#define RK_BIND_EVENT_FN(fn) [](auto &&...args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }
#define RK_BIND_EVENT_FN_CLASS(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define RK_BIND_EVENT_FN_CLASS_PTR(pointer, fn) [pointer](auto &&...args) -> decltype(auto) { return pointer->fn(std::forward<decltype(args)>(args)...); }
} // namespace Rocket
