#include "EventSystem/EventChannel.h"
#include "Math/Parameter.h"

// TODO : make a better event channel
namespace Rocket {
    void EventChannel::RegisterEvent(const EventType& type, const EventDelegate& function) {
        std::scoped_lock guard{ register_mutex_ };
        event_listener_[type].push_back(function);
        event_storage_[type].unblock();
        waiting_event_storage_[type].unblock();
        RK_INFO(Event, "Event Listener Size {}", event_listener_[type].size());
        RK_INFO(Event, "Event Queue Size {}", event_storage_.size());
        RK_INFO(Event, "Event Waiting Queue Size {}", waiting_event_storage_.size());
        //RK_INFO(Event, "Event Queue Size {}", event_storage_[current_queue_].size());
        //RK_INFO(Event, "Event Waiting Queue Size {}", waiting_event_storage_[current_queue_].size());
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
        if(this->IsEmpty()) {
            return;
        }
        else {
            auto waiting_queue = waiting_event_storage_.begin();
            auto waiting_end = waiting_event_storage_.end();
            while(waiting_queue != waiting_end) {
                EventPtr event;
                auto& queue = waiting_queue->second;
                static std::vector<EventPtr> delay_queue;
                delay_queue.clear();
                //queue.block();
                while(queue.pop(event)) {
                    double dt = step;
                    event->time_delay -= dt;
                    if(event->time_delay < 1e-6) {
                        QueueEvent(event);
                    } else {
                        delay_queue.push_back(event);
                    }
                }
                for(auto delay_event : delay_queue) {
                    queue.push(delay_event);
                }
                //queue.unblock();
                waiting_queue++;
            }
            auto event_queue = event_storage_.begin();
            auto event_end = event_storage_.end();
            while(event_queue != event_end) {
                EventPtr event;
                auto& queue = event_queue->second;
                //queue.block();
                while(queue.pop(event)) {
                    DispatchEvent(event);
                }
                //queue.unblock();
                event_queue++;
            }
        }
    }

    void EventChannel::QueueEvent(EventPtr& event) {
        if(event->time_delay > 1e-6) {
            auto event_queue = waiting_event_storage_.find(event->GetEventType());
            event_queue->second.push(event);
            //while(!event_queue->second.try_push(event));
        } else {
            auto event_queue = event_storage_.find(event->GetEventType());
            event_queue->second.push(event);
            //while(!event_queue->second.try_push(event));
        }
    }

    void EventChannel::TriggerEvent(EventPtr& event) {
        DispatchEvent(event);
    }

    void EventChannel::DispatchEvent(EventPtr& event) {
        RK_TRACE(Event, "Dispatch Event: {}", event->GetEventType());
        auto delegates = event_listener_.find(event->GetEventType())->second;
        for(auto delegate : delegates) {
            // TODO : use Task Manager
            delegate.Invoke(event);
        }
    }
}
