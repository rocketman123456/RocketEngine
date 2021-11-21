#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"

#include <ostream>
#include <sstream>

namespace Rocket {
	ElapseTimer Event::timer_s;

	Event::Event(const std::string& name) 
		: size(0), variable(nullptr), name(name) 
		{ 
		type = hash(name);
		time_stamp = timer_s.GetExactTime();
	}
	Event::Event(const std::string& name, EventDataPtr ptr, uint64_t size) 
		: size(size), variable(ptr), name(name) 
		{ 
		type = hash(name);
		time_stamp = timer_s.GetExactTime();
	}
	Event::Event(const std::string& name, EventType type) 
		: type(type), size(0), variable(nullptr) , name(name) 
		{
		time_stamp = timer_s.GetExactTime();
	}
	Event::Event(const std::string& name, EventType type, EventDataPtr ptr, uint64_t size) 
		: type(type), size(size), variable(ptr), name(name) 
		{
		time_stamp = timer_s.GetExactTime();
	}

    std::string Event::ToString() {
        std::stringstream os;
        os << "Name:[" << this->name << "]Time:[" << this->time_stamp << "]Type:[" << this->type << "]Data:{";
		for (int32_t i = 0; i < this->size; ++i) {
			switch (this->variable[i].type) {
			case Variant::TYPE_INT32:
				os << "[int32:" << this->variable[i].as_int32 << "]"; break;
			case Variant::TYPE_UINT32:
				os << "[uint32:" << this->variable[i].as_uint32 << "]"; break;
			case Variant::TYPE_FLOAT:
				os << "[float:" << this->variable[i].as_float << "]"; break;
			case Variant::TYPE_DOUBLE:
				os << "[double:" << this->variable[i].as_double << "]"; break;
			case Variant::TYPE_POINTER:
				os << "[pointer:" << this->variable[i].as_pointer << "]"; break;
			case Variant::TYPE_STRING_ID:
				os << "[string_id:" << this->variable[i].as_string_id << "]"; break;
			default:
				RK_ERROR(Event, "Unknow Event Data Type"); break;
			}
		}
		os << "}";
        std::string temp;
        os >> temp;
		return temp;
    }
}
