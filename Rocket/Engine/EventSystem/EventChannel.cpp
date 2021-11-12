#include "EventSystem/EventChannel.h"
#include "Math/Parameter.h"

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
            std::scoped_lock guard{ event_queue_mutex_[current_queue_] };

            int32_t current = current_queue_;
            int32_t previous = current_queue_;
            current = (current + 1) % EVENT_BUFFER_NUM;
            handling_queue_ = previous;
            current_queue_ = current;
        }

        if(this->IsEmpty()) {
            return;
        } else {
            std::scoped_lock guard{ event_queue_mutex_[handling_queue_] };
            {
                int32_t queue_end = waiting_queue_end_[handling_queue_];
                double dt = step;
                for(int i = 0; i < queue_end; ++i) {
                    auto event = event_storage_[handling_queue_][i];
                    if(event == nullptr) continue;
                    event->time_delay_ -= dt;
                }

                for(int i = 0; i < queue_end; ++i) {
                    auto event = event_storage_[handling_queue_][i];
                    if(event == nullptr) continue;
                    if(event_storage_[handling_queue_][i]->time_delay_ > EPS) {
                        RK_INFO(Event, "Requeue Event: {}", event->GetEventType());
                        QueueEvent(event);
                    } else {
                        event_storage_[handling_queue_][event_queue_end_[handling_queue_]] = event;
                        event_queue_end_[handling_queue_]++;
                    }
                }

                if(queue_end)
                    RK_INFO(Event, "Waiting Event: {}", queue_end);
            }

            {
                int32_t queue_end = event_queue_end_[handling_queue_];
                for(int i = 0; i < queue_end; ++i) {
                    auto event = event_storage_[handling_queue_][i];
                    DispatchEvent(event);
                }

                if(queue_end)
                    RK_INFO(Event, "Dispatch Event: {}", queue_end);
            }

            event_queue_end_[handling_queue_] = 0;
            waiting_queue_end_[handling_queue_] = 0;
        }
    }

    void EventChannel::QueueEvent(EventPtr& event) {
        if(event->time_delay_ > 1e-6) {
            std::scoped_lock guard{ event_queue_mutex_[current_queue_] };
            waiting_event_storage_[current_queue_][waiting_queue_end_[current_queue_]] = event;
            waiting_queue_end_[current_queue_]++;
        } else {
            std::scoped_lock guard{ event_queue_mutex_[current_queue_] };
            event_storage_[current_queue_][event_queue_end_[current_queue_]] = event;
            event_queue_end_[current_queue_]++;
        }
    }

    void EventChannel::TriggerEvent(EventPtr& event) {
        DispatchEvent(event);
    }

    void EventChannel::DispatchEvent(EventPtr& event) {
        RK_INFO(Event, "Dispatch Event: {}", event->GetEventType());
        auto delegates = event_listener_.find(event->GetEventType())->second;
        for(auto delegate : delegates) {
            delegate.Invoke(event);
        }
    }
}
