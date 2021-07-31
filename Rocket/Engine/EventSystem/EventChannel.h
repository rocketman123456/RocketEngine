#pragma once
#include "EventSystem/Event.h"

#include <unordered_map>
#include <functional>
#include <string>

namespace Rocket {
    using EventMap = std::unordered_map<EventType, std::function<bool(EventSPtr)>>;

    class EventChannel {
    public:
        explicit EventChannel();    // Auto Generate Name
        explicit EventChannel(const std::string name);    // Custom Name
        virtual ~EventChannel() = default;
        void RegisterEvent(EventType type);
        void UnregisterEvent(EventType type);

        void QueueEvent();
        void DispatchEvent();
        void TriggerEvent();

        inline const std::string& GetName() { return name_; }
    private:
        std::string name_;
        EventMap event_map_;
    };
}
