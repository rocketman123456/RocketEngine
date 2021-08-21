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
            auto delay_queue = std::vector<EventPtr>();
            auto event_queue = event_storage_.begin();
            auto end_queue = event_storage_.end();
            while(event_queue != end_queue) {
                EventPtr event;
                auto& queue = event_queue->second;
                //RK_TRACE(Event, "Queue Event Count {}", queue.size());
                //queue_.block();
                while(queue.pop(event)) {
                    double dt = step;
                    event->time_delay_ = event->time_delay_ - dt;
                    if(event->time_delay_ > 0) {
                        delay_queue.push_back(event);
                        continue;
                    }
                    else {
                        DispatchEvent(event);
                    }
                }
                //queue_.unblock();
                event_queue++;
            }
            //if(delay_queue.size())
            //    RK_TRACE(Event, "Delayed Event Count {}", delay_queue.size());
            for(auto event : delay_queue) {
                QueueEvent(event);
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
