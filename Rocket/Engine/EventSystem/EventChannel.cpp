#include "EventSystem/EventChannel.h"
#include "Math/Parameter.h"

// TODO : make a better event channel
namespace Rocket {
    void EventChannel::RegisterEvent(const EventType& type, const EventDelegate& function) {
        std::scoped_lock guard{ register_mutex_ };
        event_listener_[type].push_back(function);
        RK_INFO(Event, "Event Listener Size {}", event_listener_[type].size());
        RK_INFO(Event, "Event Queue Size {}", event_storage_[current_queue_].size());
        RK_INFO(Event, "Event Waiting Queue Size {}", waiting_event_storage_[current_queue_].size());
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
            std::scoped_lock guard{ 
                event_queue_mutex_single_
            };
            handling_queue_ = current_queue_;
            current_queue_ = (current_queue_ + 1) % EVENT_BUFFER_NUM;
        }

        if(this->IsEmpty()) {
            return;
        } else {
            // Handle Waiting Event / Delayed Event
            int32_t queue_end = waiting_queue_end_[handling_queue_];
            double dt = step;
            for(int i = 0; i < queue_end; ++i) {
                auto event = waiting_event_storage_[handling_queue_][i];
                //if(event == nullptr) continue;
                event->time_delay_ -= dt;
                if(waiting_event_storage_[handling_queue_][i]->time_delay_ > EPS) {
                    QueueEvent(event);
                } else {
                    event_storage_[handling_queue_][event_queue_end_[handling_queue_]] = event;
                    event_queue_end_[handling_queue_]++;
                }
            }

            {
                // Handle Normal Event
                int32_t queue_end = event_queue_end_[handling_queue_];
                if(queue_end)
                    RK_TRACE(Event, "Dispatch Event Count: {}", queue_end);
                for(int i = 0; i < queue_end; ++i) {
                    auto event = event_storage_[handling_queue_][i];
                    DispatchEvent(event);
                }
            }

            event_queue_end_[handling_queue_] = 0;
            waiting_queue_end_[handling_queue_] = 0;
        }
    }

    void EventChannel::QueueEvent(EventPtr& event) {
        if(event->time_delay_ > EPS) {
            std::scoped_lock guard{ event_queue_mutex_single_ };
            if(waiting_queue_end_[current_queue_] >= MAX_EVENT_NUM)
                RK_ERROR(Event, "Too Much Events At This Moment");
            waiting_event_storage_[current_queue_][waiting_queue_end_[current_queue_]] = event;
            waiting_queue_end_[current_queue_]++;
        } else {
            std::scoped_lock guard{ event_queue_mutex_single_ };
            if(waiting_queue_end_[current_queue_] >= MAX_EVENT_NUM)
                RK_ERROR(Event, "Too Much Events At This Moment");
            event_storage_[current_queue_][event_queue_end_[current_queue_]] = event;
            event_queue_end_[current_queue_]++;
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
