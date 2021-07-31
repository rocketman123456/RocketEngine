#include "EventSystem/EventChannel.h"

namespace Rocket {
    EventChannel::EventChannel() {
    }

    EventChannel::EventChannel(const std::string name) : name_(name) {
    }

    void EventChannel::RegisterEvent(EventType type, void* function) {
    }

    void EventChannel::UnregisterEvent(EventType type, void* function) {
    }

    void EventChannel::Tick(TimeStep step) {
    }

    void EventChannel::QueueEvent(EventSPtr event) {
    }

    void EventChannel::DispatchEvent(EventSPtr event) {
    }

    void EventChannel::TriggerEvent(EventSPtr event) {
    }
}
