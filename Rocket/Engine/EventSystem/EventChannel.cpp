#include "EventSystem/EventChannel.h"
#include "Math/Parameter.h"

namespace Rocket {
    void EventChannel::RegisterEvent(const EventType& type, const EventDelegate& function) {
        std::scoped_lock guard{ register_mutex_ };
        event_listener_[type].push_back(function);
        RK_INFO(Event, "Event Listener Size {}", event_listener_[type].size());
    }

    void EventChannel::UnregisterEvent(const EventType& type, const EventDelegate& function) {
        std::scoped_lock guard{ register_mutex_ };
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
        {
            std::unique_lock guard{ queue_change_mutex_ };
            //RK_TRACE(Event, "Event Queue Size {}", event_storage_[current_queue_].size());
            //RK_TRACE(Event, "Event Waiting Queue Size {}", waiting_storage_[current_queue_].size());
            handling_queue_ = current_queue_;
            current_queue_ = (current_queue_ + 1) % EVENT_BUFFER_NUM;
        }

        for(auto event : waiting_storage_[handling_queue_]) {
            double dt = step;
            event->time_delay -= dt;
            if(event->time_delay > EPS) 
                QueueEvent(event);
            else {
                event_storage_[handling_queue_].push_back(event);
            }
        }

        waiting_storage_[handling_queue_].clear();

        for(auto event : event_storage_[handling_queue_]) {
            DispatchEvent(event);
        }

        event_storage_[handling_queue_].clear();
    }

    void EventChannel::QueueEvent(EventPtr& event) {
        if(event->time_delay > 1e-6) {
            std::unique_lock guard{ queue_change_mutex_ };
            waiting_storage_[current_queue_].push_back(event);
        } else {
            std::unique_lock guard{ queue_change_mutex_ };
            event_storage_[current_queue_].push_back(event);
        }
    }

    void EventChannel::TriggerEvent(EventPtr& event) {
        DispatchEvent(event);
    }

    void EventChannel::DispatchEvent(EventPtr& event) {
        std::scoped_lock guard{ register_mutex_ };
        RK_TRACE(Event, "Dispatch Event: {}", event->ToString());
        auto delegates = event_listener_.find(event->GetEventType())->second;
        for(auto delegate : delegates) {
            delegate.Invoke(event);
        }
    }
}
