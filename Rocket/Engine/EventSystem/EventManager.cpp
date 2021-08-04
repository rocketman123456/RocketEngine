#include "EventSystem/EventManager.h"
#include "Log/Log.h"

namespace Rocket {
    int EventManager::Initialize() {
        channels_.clear();
        event_channel_map_.clear();
        return 0;
    }

    void EventManager::Finalize() {
        channels_.clear();
        event_channel_map_.clear();
    }

    void EventManager::Tick(TimeStep step) {
        // Loop Through Every Event Channel And Dispatch Events
        for(auto channel : channels_) {
            if(channel.second->IsEmpty()) {
                continue;
            }
            else {
                channel.second->Tick(step);
            }
        }
    }

    void EventManager::QueueEvent(EventPtr& event) {
        EventType type = event->GetEventType();
        event_channel_map_.find(type);
    }

    void EventManager::TriggerEvent(EventPtr& event) {
        EventType type = event->GetEventType();
        event_channel_map_.find(type);
    }

    void EventManager::AddChannel(EventType type, ChannelPtr channel) {
        auto it = event_channel_map_.find(type);
        if(it == event_channel_map_.end()) {
            event_channel_map_[type] = {};
            event_channel_map_[type][hash(channel->GetName())] = channel;
        }
        else {
            auto& map = it->second;
            map[hash(channel->GetName())] = channel;
        }
        channels_[hash(channel->GetName())] = channel;
    }

    void EventManager::RemoveChannel(EventType type, const std::string& name) {
        auto it = event_channel_map_.find(type);
        if(it == event_channel_map_.end()) {
            RK_INFO(Event, "Cannot Find Channel {} For Event {}, Nothing to Remove", name, type);
        }
        else {
            auto& map = it->second;
            auto ch = map.find(hash(name));
            if(ch == map.end()) {
                RK_INFO(Event, "Cannot Find Channel: {}, Nothing to Remove", name);
            }
            else {
                map.erase(ch);
            }
        }
    }

    void EventManager::RemoveChannel(const std::string& name) {
        for(auto it : event_channel_map_) {
            auto& channel = it.second;
            auto result = channel.find(hash(name));
            if(result != channel.end()) {
                channel.erase(result);
            }
        }
        auto it = channels_.find(hash(name));
        if(it != channels_.end()) {
            channels_.erase(it);
        }
    }

    void EventManager::AddEventListener(const EventDelegate& function, const EventType& type, const std::string& channel_name) {
        auto type_map = event_channel_map_.find(type);
        if(type_map != event_channel_map_.end()){
            auto it = type_map->second.find(hash(channel_name));
            if(it != type_map->second.end()) {
                it->second->RegisterEvent(type, function);
            }
            else {
                RK_WARN(Event, "Cannot Find Channel : {}", channel_name);
            }
        }
        else {
            RK_WARN(Event, "Cannot Find Channel {} In Event Channel Map", channel_name);
        }
    }

    void EventManager::RemoveEventListener(const EventDelegate& function, const EventType& type, const std::string& channel_name) {
        auto type_map = event_channel_map_.find(type);
        if(type_map != event_channel_map_.end()){
            auto it = type_map->second.find(hash(channel_name));
            if(it != type_map->second.end()) {
                it->second->UnregisterEvent(type, function);
            }
            else {
                RK_WARN(Event, "Cannot Find Channel : {}", channel_name);
            }
        }
        else {
            RK_WARN(Event, "Cannot Find Channel {} In Event Channel Map", channel_name);
        }
    }

    void EventManager::AddEventListener(const EventDelegate& function, const std::string& name, const std::string& channel_name) {
        EventType type = hash(name);
        auto type_map = event_channel_map_.find(type);
        if(type_map != event_channel_map_.end()){
            auto it = type_map->second.find(hash(channel_name));
            if(it != type_map->second.end()) {
                it->second->RegisterEvent(type, function);
            }
            else {
                RK_WARN(Event, "Cannot Find Channel : {}", channel_name);
            }
        }
        else {
            RK_WARN(Event, "Cannot Find Channel {} In Event Channel Map", channel_name);
        }
    }

    void EventManager::RemoveEventListener(const EventDelegate& function, const std::string& name, const std::string& channel_name) {
        EventType type = hash(name);
        auto type_map = event_channel_map_.find(type);
        if(type_map != event_channel_map_.end()){
            auto it = type_map->second.find(hash(channel_name));
            if(it != type_map->second.end()) {
                it->second->UnregisterEvent(type, function);
            }
            else {
                RK_WARN(Event, "Cannot Find Channel : {}", channel_name);
            }
        }
        else {
            RK_WARN(Event, "Cannot Find Channel {} In Event Channel Map", channel_name);
        }
    }
}
