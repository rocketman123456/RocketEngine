#include "EventSystem/Event.h"

#include <ostream>
#include <sstream>

namespace Rocket {
    std::string Event::ToString() {
        std::stringstream os;
        os << "Name:[" << this->name_ << "]Time:[" << this->time_stamp_ << "]Type:[" << this->type_ << "]Data:{";
		for (int32_t i = 0; i < this->size_; ++i) {
			switch (this->variable_[i].type) {
			case Variant::TYPE_INT32:
				os << "[int32:" << this->variable_[i].as_int32 << "]"; break;
			case Variant::TYPE_UINT32:
				os << "[uint32:" << this->variable_[i].as_uint32 << "]"; break;
			case Variant::TYPE_FLOAT:
				os << "[float:" << this->variable_[i].as_float << "]"; break;
			case Variant::TYPE_DOUBLE:
				os << "[double:" << this->variable_[i].as_double << "]"; break;
			case Variant::TYPE_POINTER:
				os << "[pointer:" << this->variable_[i].as_pointer << "]"; break;
			case Variant::TYPE_STRING_ID:
				os << "[string_id:" << this->variable_[i].as_string_id << "]"; break;
			default:
				RK_ERROR(Event, "Unknow Event Data Type"); break;
			}
		}
		os << "}";
        std::string temp;
        os >> temp;
		return std::move(temp);
    }
}