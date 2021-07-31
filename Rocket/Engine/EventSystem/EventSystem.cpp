#include "EventSystem/EventSystem.h"
#include "Log/Log.h"

namespace Rocket {
    int EventManager::Initialize() {
        return 0;
    }

    void EventManager::Finalize() {
    }

    void EventManager::Tick(TimeStep step) {
        // Loop Through Every Event Channel And Dispatch Events
    }

    void EventManager::AddChannel(EventType type, std::shared_ptr<EventChannel> channel) {
        // TODO : use hash string id to replace string, for better compare performance
        {
            auto it = event_channel_map_.find(type);
            if(it == event_channel_map_.end()) {
                it->second = {};
                it->second[channel->GetName()] = channel;
            }
            else {
                auto& map = it->second;
                // avoid re-insert same channel to same type
                auto ch = map.find(channel->GetName());
                if(ch == map.end()) {
                    map[channel->GetName()] = channel;
                }
            }
        }
        {
            auto it = channels_.find(channel->GetName());
            if(it == channels_.end()) {
                channels_[channel->GetName()] = channel;
            }
        }
    }

    void EventManager::RemoveChannel(EventType type, const std::string name) {
        {
            auto it = event_channel_map_.find(type);
            if(it == event_channel_map_.end()) {
                RK_INFO(Event, "Cannot Find Channel {} For Event {}, Nothing to Remove", name, type);
            }
            else {
                auto& map = it->second;
                auto ch = map.find(name);
                if(ch == map.end()) {
                    RK_INFO(Event, "Cannot Find Channel: {}, Nothing to Remove", name);
                }
                else {
                    map.erase(ch);
                }
            }
        }
        {
            // TODO : maybe should not use this auto delete, may cause some error
            //auto it = channels_.find(name);
            //if(it == channels_.end()) {
            //    RK_INFO(Event, "Cannot Find Channel: {}, Nothing to Remove", name);
            //}
            //else if(it->second->GetIsEmpty()) {
            //    // Remove Empty Event Channel
            //    channels_.erase(it);
            //}
        }
    }

    void EventManager::AddEventListener(void* function, EventType type, const std::string& channel_name) {

    }

    void EventManager::RemoveEventListener(void* function, EventType type, const std::string& channel_name) {

    }
}
