#pragma once
#include "EventSystem/Event.h"
#include "Containers/Queue/BlockingQueue.h"
#include "Utils/TimeStep.h"

namespace Rocket {
    class EventChannel;
    using ChannelPtr = std::shared_ptr<EventChannel>;

    class EventChannel {
    public:
        explicit EventChannel();                                        // Auto Generate Name
        explicit EventChannel(const std::string name): name_(name) {}   // Custom Name
        virtual ~EventChannel() = default;

        void RegisterEvent(const EventType& type, const EventDelegate& function);
        void UnregisterEvent(const EventType& type, const EventDelegate& function);

        virtual void Tick(TimeStep step);
        virtual void QueueEvent(EventPtr& event);
        virtual void DispatchEvent(EventPtr& event);
        virtual void TriggerEvent(EventPtr& event);

        inline std::string GetName() const { return name_; }
        inline bool IsEmpty() const { return event_listener_.empty(); }
    private:
        std::string name_;
        EventListener event_listener_;
    };
}
