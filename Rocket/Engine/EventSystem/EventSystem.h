#pragma once
#include "Pattern/IRuntimeModule.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventChannel.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <set>
#include <memory>

namespace Rocket {
    // TODO : use custom data structure instead
    using ChannelMap = std::unordered_map<std::string, std::shared_ptr<EventChannel>>;
    using EventChannelMap = std::unordered_map<EventType, ChannelMap>;

    class EventManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(EventManager);
    public:
        EventManager() = default;
        virtual ~EventManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        void AddChannel(EventType type, std::shared_ptr<EventChannel> channel);
        void RemoveChannel(EventType type, const std::string name);

        void AddEventListener(void* function, EventType type, const std::string& channel_name);
        void RemoveEventListener(void* function, EventType type, const std::string& channel_name);
    private:
        ChannelMap channels_;
        EventChannelMap event_channel_map_;
    };
}
