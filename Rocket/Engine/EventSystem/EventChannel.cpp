#include "EventSystem/EventChannel.h"

namespace Rocket {
    EventChannel::EventChannel() {
        // TODO : auto generate name and hash-type
    }

    void EventChannel::RegisterEvent(const EventType& type, const EventDelegate& function) {
        event_listener_[type].push_back(function);
        event_storage_[type].unblock();
        RK_INFO(Event, "Event Listener Size {}", event_listener_[type].size());
        RK_INFO(Event, "Event Queue Size {}", event_storage_.size());
    }

    void EventChannel::UnregisterEvent(const EventType& type, const EventDelegate& function) {
        auto it = event_listener_.find(type);
        if(it == event_listener_.end()) {
            RK_WARN(Event, "Cannot Find Event Delegate");
        }
        else {
            auto delegate = it->second.begin();
            auto end_element = it->second.end();
            while(delegate != end_element) {
                if(*delegate == function) {
                    it->second.erase(delegate);
                    break;
                }
            }
        }
    }

    void EventChannel::Tick(TimeStep step) {
        if(this->IsEmpty()) {
            return;
        }
        else {
            auto event_queue = event_storage_.begin();
            auto end_queue = event_storage_.end();
            while(event_queue != end_queue) {
                EventPtr event;
                auto& queue_ = event_queue->second;
                RK_INFO(Event, "Queue Event Count {}", queue_.size());
                //queue_.block();
                while(queue_.pop(event)) {
                    DispatchEvent(event);
                }
                //queue_.unblock();
                event_queue++;
            }
        }
    }

    void EventChannel::QueueEvent(EventPtr& event) {
        auto event_queue = event_storage_.find(event->GetEventType());
        event_queue->second.push(event);
    }

    void EventChannel::TriggerEvent(EventPtr& event) {
        DispatchEvent(event);
    }

    void EventChannel::DispatchEvent(EventPtr& event) {
        auto delegates = event_listener_.find(event->GetEventType())->second;
        for(auto delegate : delegates) {
            delegate.Invoke(event);
        }
    }
}
