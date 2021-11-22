#pragma once
#include "EventSystem/Event.h"
//#include "Containers/Queue/UnboundedQueue.h"
#include "Utils/TimeStep.h"

#include <list>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <cstdint>

namespace Rocket {
    constexpr int32_t EVENT_BUFFER_NUM = 2;
    constexpr int32_t MAX_EVENT_NUM = 1 * 100 * 100;
    using EventListener = std::unordered_map<EventType, std::vector<EventDelegate>>;
    using EventStorage = std::vector<EventPtr>;

    class EventChannel {
    public:
        explicit EventChannel() {}                                       // Auto Generate Name
        explicit EventChannel(const std::string& name): name_(name) {}   // Custom Name
        virtual ~EventChannel() = default;

        void RegisterEvent(const EventType& type, const EventDelegate& function);
        void UnregisterEvent(const EventType& type, const EventDelegate& function);

        virtual void Tick(TimeStep step);
        
        virtual void QueueEvent(EventPtr& event);
        virtual void TriggerEvent(EventPtr& event);
        virtual void DispatchEvent(EventPtr& event);

        inline std::string GetName() const { return name_; }
        inline bool IsEmpty() const { return event_listener_.empty(); }
    private:
        std::string name_;

        std::mutex register_mutex_;
        EventListener event_listener_;

        std::mutex queue_change_mutex_;
        int32_t current_queue_ = 0;
        int32_t handling_queue_ = 0;
        EventStorage event_storage_[EVENT_BUFFER_NUM];
        EventStorage waiting_storage_[EVENT_BUFFER_NUM];
    };

    using ChannelPtr = std::shared_ptr<EventChannel>;
}
