#pragma once
#include "Pattern/IRuntimeModule.h"
#include "Pattern/Singleton.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventChannel.h"
#include "Utils/Timer.h"

#include <set>
#include <memory>

namespace Rocket {
    // TODO : use custom data structure instead
    //using ChannelMap = std::unordered_map<std::string, ChannelPtr>;
    using ChannelMap = std::unordered_map<uint64_t, ChannelPtr>;
    using EventChannelMap = std::unordered_map<EventType, ChannelMap>;

    class EventManager : _implements_ IRuntimeModule, _implements_ AbstractSingleton<EventManager> {
        RUNTIME_MODULE_TYPE(EventManager);
    public:
        EventManager() = default;
        virtual ~EventManager() = default;

        [[nodiscard]] virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

        void AddChannel(const std::string& name, ChannelPtr channel);
        void AddChannel(EventType type, ChannelPtr channel);
        // Remove Channel From Special Event Type, Will NOT Remove Empty Channel
        void RemoveChannel(EventType type, const std::string& name);
        // Remove Channel Completely
        void RemoveChannel(const std::string& name);

        void QueueEvent(EventPtr& event);
        void TriggerEvent(EventPtr& event);

        void AddEventListener(const EventDelegate& function, const EventType& type, const std::string& channel_name);
        void RemoveEventListener(const EventDelegate& function, const EventType& type, const std::string& channel_name);
        void AddEventListener(const EventDelegate& function, const std::string& name, const std::string& channel_name);
        void RemoveEventListener(const EventDelegate& function, const std::string& name, const std::string& channel_name);
    private:
        ChannelMap channels_;
        EventChannelMap event_channel_map_;
    };
}

#define REGISTER_DELEGATE_CLASS(class,function,instance,name,ch_name) do {\
    Rocket::EventDelegate delegate; \
    delegate.Bind<class,&function>(instance); \
    Rocket::EventManager::Instance()->AddEventListener(delegate, name, ch_name); } while(0);
#define REGISTER_DELEGATE_FN(function,name,ch_name) do {\
    Rocket::EventDelegate delegate; \
    delegate.Bind<&function>(); \
    Rocket::EventManager::Instance()->AddEventListener(delegate, name, ch_name); } while(0);
