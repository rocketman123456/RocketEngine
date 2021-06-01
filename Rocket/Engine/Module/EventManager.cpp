#include "Module/EventManager.h"
#include "Module/WindowManager.h"

#if defined(RK_DESKTOP)
//#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#endif

[[maybe_unused]] static Rocket::default_allocator<Rocket::Event> g_event_allocator;

namespace Rocket {
    EventManager* EventManager::instance_ = nullptr;
    ElapseTimer* g_EventTimer = nullptr;

    EventManager::EventManager(bool global) : global_(global) {
        if(global_) {
            RK_CORE_ASSERT(!instance_, "Global EventManager already exists!");
            instance_ = this;
        }
    }

    int32_t EventManager::Initialize() {
        g_EventTimer = new ElapseTimer();
        g_EventTimer->Start();

        active_event_queue_ = 0;
#if defined(RK_DESKTOP)
        window_data_.title = "Rocket";
#endif

        SetupListener();

#if defined(RK_DESKTOP)
        //glfwSetWindowUserPointer(window_handle_, &window_data_);
#endif

        SetupCallback();

        timer_.Start();

        return 0;
    }

    void EventManager::SetupCallback() {

    }

    void EventManager::SetupListener() {
        SetEventCallback(std::bind(&EventManager::OnEvent, this, std::placeholders::_1));
    }

    void EventManager::Finalize() {
        delete g_EventTimer;
    }

    void EventManager::OnEvent(EventPtr& event) {
        bool ret = false;
        //RK_EVENT_TRACE("Callback Event {0}", *event);
        auto event_type = event->GetEventType();
        auto id_1 = "window_close"_hash;//GlobalHashTable::HashString("Event"_hash, "window_close");
        auto id_2 = "window_resize"_hash;//GlobalHashTable::HashString("Event"_hash, "window_resize");
        if(event_type == id_1 || event_type == id_2)
            ret = QueueEvent(event);
        else
            ret = TriggerEvent(event);
        //RK_EVENT_TRACE("On Event Callback Result : {}", ret);
    }

    void EventManager::Tick(TimeStep ts) {
        Rocket::g_EventTimer->MarkLapping();

        //glfwPollEvents();

        timer_.MarkLapping();
        bool result = Update();
        if(!result) {
            RK_EVENT_WARN("EventManager Process Unfinish!");
        }
    }

    bool EventManager::Update(uint64_t maxMillis) {
        double currMs = timer_.GetElapsedTime();
        double maxMs = currMs + maxMillis;

        // This section added to handle events from other threads.
        //EventPtr pRealtimeEvent;
        //while (event_thread_queue_.try_pop(pRealtimeEvent))
        //{
        //    QueueEvent(pRealtimeEvent);
        //    currMs = timer_.GetElapsedTime();
        //    if (currMs >= maxMs)
        //    {
        //        RK_EVENT_WARN("A realtime process is spamming the event manager! {}", *pRealtimeEvent);
        //    }
        //}

        // swap active queues and clear the new queue after the swap
        int queueToProcess = active_event_queue_;
        active_event_queue_ = (active_event_queue_ + 1) % EVENTMANAGER_NUM_QUEUES;
        event_queue_[active_event_queue_].clear();

        //RK_EVENT_INFO("Processing Event Queue {0} - {1} events to process", queueToProcess, event_queue_[queueToProcess].size());

        // Process the queue
        while (!event_queue_[queueToProcess].empty()) {
            // pop the front of the queue
            EventPtr pEvent = event_queue_[queueToProcess].front();
            event_queue_[queueToProcess].pop_front();
            //RK_EVENT_TRACE("\tProcessing Event {0}", pEvent->GetName());

            const EventType& eventType = pEvent->GetEventType();

            // find all the delegate functions registered for this event
            auto findIt = event_listener_.find(eventType);
            if (findIt != event_listener_.end()) {
                const EventListenerList& eventListeners = findIt->second;
                //RK_EVENT_TRACE("\tFound {0} delegates", (unsigned long)eventListeners.size());

                // call each listener
                for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
                    EventListenerDelegate listener = (*it);
                    //RK_EVENT_TRACE("\tSending event {0} to delegate", pEvent->GetName());
                    bool processed = listener(pEvent);
                    if (processed)
                        break;
                }
            }

            // check to see if time ran out
            currMs = timer_.GetElapsedTime();
            if (currMs >= maxMs) {
                RK_EVENT_TRACE("Aborting event processing; time ran out");
                break;
            }
        }
            
        // If we couldn't process all of the events, push the remaining events to the new active queue.
        // Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
        bool queueFlushed = (event_queue_[queueToProcess].empty());
        if (!queueFlushed) {
            while (!event_queue_[queueToProcess].empty()) {
                EventPtr pEvent = event_queue_[queueToProcess].back();
                event_queue_[queueToProcess].pop_back();
                event_queue_[active_event_queue_].push_back(pEvent);
            }
        }
            
        return queueFlushed;
    }

    bool EventManager::AddListener(const EventListenerDelegate& eventDelegate, const EventType& type) {
        EventListenerList& eventListenerList = event_listener_[type];  // this will find or create the entry
        for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it) {
            // Must use function pointer as listener
            if (eventDelegate == *it) {
                RK_EVENT_WARN("Attempting to double-register a delegate");
                return false;
            }
        }
        eventListenerList.push_back(eventDelegate);
        return true;
    }

    bool EventManager::RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) {
        bool success = false;
        auto findIt = event_listener_.find(type);
        if (findIt != event_listener_.end()) {
            EventListenerList& listeners = findIt->second;
            for (auto it = listeners.begin(); it != listeners.end(); ++it) {
                // Must use function pointer as listener
                if (eventDelegate == (*it)) {
                    listeners.erase(it);
                    RK_EVENT_TRACE("Successfully removed delegate function from event type {0}", type);
                    success = true;
                    break;
                }
            }
        }
        return success;
    }

    bool EventManager::TriggerEvent(EventPtr& event) const {
        //RK_EVENT_TRACE("Trigger Event : {}", event->GetName());
        bool processed = false;
        auto findIt = event_listener_.find(event->GetEventType());
        if (findIt != event_listener_.end()) {
            const EventListenerList& eventListenerList = findIt->second;
            for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it) {
                auto listener = (*it);
                //RK_EVENT_TRACE("Trigger Event {0} to delegate.", event->GetName());
                processed = listener(event);  // call the delegate
                if (processed)
                    break;
            }
        }
        return processed;
    }

    bool EventManager::QueueEvent(const EventPtr& event) {
        RK_CORE_ASSERT(active_event_queue_ >= 0, "EventManager Active Queue Error");
        RK_CORE_ASSERT(active_event_queue_ < EVENTMANAGER_NUM_QUEUES, "EventManager Active Queue Error");

        //RK_EVENT_TRACE("Attempting to queue event: {0}", event->GetName());

        auto findIt = event_listener_.find(event->GetEventType());
        if (findIt != event_listener_.end()) {
            event_queue_[active_event_queue_].push_back(event);
            //RK_EVENT_TRACE("Successfully queued event: {0}", event->GetName());
            return true;
        }
        else {
            //RK_EVENT_TRACE("Skipping event since there are no delegates registered: {0}", event->GetName());
            return false;
        }
    }

    bool EventManager::ThreadSafeQueueEvent(const EventPtr& event) {
        //event_thread_queue_.push(event);
        return true;
    }

    bool EventManager::AbortEvent(const EventType& type, bool allOfType) {
        RK_CORE_ASSERT(active_event_queue_ >= 0, "EventManager Active Queue Error");
        RK_CORE_ASSERT(active_event_queue_ < EVENTMANAGER_NUM_QUEUES, "EventManager Active Queue Error");

        bool success = false;
        EventListenerMap::iterator findIt = event_listener_.find(type);

        if (findIt != event_listener_.end()) {
            EventQueue& eventQueue = event_queue_[active_event_queue_];
            auto it = eventQueue.begin();
            while (it != eventQueue.end()) {
                auto thisIt = it;
                ++it;

                if ((*thisIt)->GetEventType() == type) {
                    eventQueue.erase(thisIt);
                    success = true;
                    if (!allOfType)
                        break;
                }
            }
        }

        return success;
    }
}
