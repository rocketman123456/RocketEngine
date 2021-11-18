#pragma once
#include "EventSystem/Event.h"
#include "Utils/TimeStep.h"

#include <vector>
#include <array>
#include <list>
#include <mutex>
#include <thread>

namespace Rocket {
    constexpr int32_t EVENT_BUFFER_NUM = 2;
    constexpr int32_t MAX_EVENT_NUM = 100 * 100 * 100;
    class EventChannel;
    using ChannelPtr = std::shared_ptr<EventChannel>;
    using EventListener = std::unordered_map<EventType, std::vector<EventDelegate>>;

    class EventChannel {
    public:
        explicit EventChannel() = default;                               // Auto Generate Name
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
        std::mutex event_queue_mutex_single_;
        std::mutex event_queue_mutex_[EVENT_BUFFER_NUM];
        EventListener event_listener_;
        std::array<EventPtr, MAX_EVENT_NUM> event_storage_[EVENT_BUFFER_NUM] = {};
        std::array<EventPtr, MAX_EVENT_NUM> waiting_event_storage_[EVENT_BUFFER_NUM] = {};
        int32_t current_queue_ = 0;
        int32_t handling_queue_ = 0;
        int32_t event_queue_end_[EVENT_BUFFER_NUM] = {0};
        int32_t waiting_queue_end_[EVENT_BUFFER_NUM] = {0};
    };
}
