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
    using EventChannelMap = std::unordered_map<EventType, std::unordered_map<std::string, std::unique_ptr<EventChannel>>>;

    class EventManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(EventManager);
    public:
        EventManager() = default;
        virtual ~EventManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        void AddChannel(EventType type, std::unique_ptr<EventChannel> channel);
        void RemoveChannel(EventType type, const std::string name);
    private:
        EventChannelMap event_channels_;
    };
}
