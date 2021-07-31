#include "EventSystem/EventChannel.h"

namespace Rocket {
    EventChannel::EventChannel() {
    }

    EventChannel::EventChannel(const std::string name) : name_(name) {
    }

    void EventChannel::QueueEvent() {
    }

    void EventChannel::DispatchEvent() {
    }

    void EventChannel::TriggerEvent(){
    }
}
