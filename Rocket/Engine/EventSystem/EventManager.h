#pragma once
#include "Pattern/IRuntimeModule.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventChannel.h"

#include <set>
#include <memory>

namespace Rocket {
    // TODO : use custom data structure instead
    //using ChannelMap = std::unordered_map<std::string, ChannelPtr>;
    using ChannelMap = std::unordered_map<uint64_t, ChannelPtr>;
    using EventChannelMap = std::unordered_map<EventType, ChannelMap>;

    // TODO : use hash string id to replace string, for better compare performance
    class EventManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(EventManager);
    public:
        EventManager() = default;
        virtual ~EventManager() = default;

        [[nodiscard]] virtual int Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep step) final;

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

    extern EventManager* g_EventManager;
}

#define REGISTER_DELEGATE_CLASS(class,function,instance,name,ch_name) {\
    Rocket::EventDelegate delegate; \
    delegate.Bind<class,&function>(instance); \
    Rocket::g_EventManager->AddEventListener(delegate, name, ch_name); }
#define REGISTER_DELEGATE_FN(function,name,ch_name) {\
    Rocket::EventDelegate delegate; \
    delegate.Bind<&function>(); \
    Rocket::g_EventManager->RemoveEventListener(delegate, name, ch_name); }
