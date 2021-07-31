#pragma once
#include "EventSystem/Event.h"
#include "Containers/Queue/BlockingQueue.h"
#include "Utils/TimeStep.h"

#include <unordered_map>
#include <functional>
#include <string>

namespace Rocket {
    using EventMap = std::unordered_map<EventType, std::function<bool(EventSPtr)>>;

    class EventChannel {
    public:
        explicit EventChannel();                        // Auto Generate Name
        explicit EventChannel(const std::string name);  // Custom Name
        virtual ~EventChannel() = default;

        void RegisterEvent(EventType type, void* function);
        void UnregisterEvent(EventType type, void* function);

        virtual void Tick(TimeStep step);
        virtual void QueueEvent(EventSPtr event);
        virtual void DispatchEvent(EventSPtr event);
        virtual void TriggerEvent(EventSPtr event);

        inline const std::string& GetName() const { return name_; }
        inline bool GetIsEmpty() const { return event_map_.empty(); }
    private:
        std::string name_;
        EventMap event_map_;
    };
}
