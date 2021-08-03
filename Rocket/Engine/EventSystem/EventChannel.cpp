#include "EventSystem/EventChannel.h"

namespace Rocket {
    EventChannel::EventChannel() {
    }

    void EventChannel::RegisterEvent(const EventType& type, const EventDelegate& function) {
    }

    void EventChannel::UnregisterEvent(const EventType& type, const EventDelegate& function) {
    }

    void EventChannel::Tick(TimeStep step) {
        if(this->IsEmpty()) {
            return;
        }
        else {
            // Handle Events
        }
    }

    void EventChannel::QueueEvent(EventPtr& event) {
    }

    void EventChannel::DispatchEvent(EventPtr& event) {
    }

    void EventChannel::TriggerEvent(EventPtr& event) {
    }
}
