## 9. 事件系统<br>

## 9.1. 事件系统概述<br>
`Event.h`
```
#pragma once
#include "Utils/Variant.h"
#include "Utils/Delegate.h"
#include "Utils/Hashing.h"
#include "Utils/Timer.h"
#include "Log/Log.h"

#include <vector>
#include <memory>
#include <cstdint>
#include <string>
#include <cassert>
#include <functional>
#include <unordered_map>

namespace Rocket {
    // Forward Declariation
    struct Event;

	using EventType = uint64_t;
    using EventDataPtr = Variant*;
    using EventPtr = std::shared_ptr<Event>;
    using EventDelegate = Delegate<bool(EventPtr&)>;
    using EventFunction = bool (*) (EventPtr&);
    //typedef bool(*EventFunction)(EventPtr&);

    struct Event {
        explicit Event(const std::string& name);
        explicit Event(const std::string& name, EventType type);
		explicit Event(const std::string& name, EventDataPtr ptr, uint64_t size);
        explicit Event(const std::string& name, EventType type, EventDataPtr ptr, uint64_t size);
        Event(const Event& event) {
            name_ = event.name_;
            type_ = event.type_;
            size_ = event.size_;
            variable_ = new Variant[size_];
            memcpy(variable_, event.variable_, sizeof(Variant) * size_);
            time_stamp_ = event.time_stamp_;
            time_delay_ = event.time_delay_;
        }
        Event(Event&& event) {
            name_ = event.name_;
            type_ = event.type_;
            size_ = event.size_;
            variable_ = event.variable_;
            time_stamp_ = event.time_stamp_;
            time_delay_ = event.time_delay_;
        }
        virtual ~Event() { if(variable_ != nullptr) delete[] variable_; }

        void Tick(double dt) { time_delay_ -= dt; }
        bool Ready() { return time_delay_ <= 1e-3; }

        EventType GetEventType() const { return type_; }
        int32_t GetInt32(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_int32; }
		uint32_t GetUInt32(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_uint32; }
		float GetFloat(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_float; }
		double GetDouble(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_double; }
		bool GetBool(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_bool; }
		void* GetPointer(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_pointer; }
		string_id GetStringId(uint64_t index) { assert(index < size_ && "event index error"); return variable_[index].as_string_id; }
        const EventDataPtr GetRawData() const { return variable_; }
        std::string ToString();

        Event& operator = (const Event& other) {
            name_ = other.name_;
            type_ = other.type_;
            size_ = other.size_;
            variable_ = new Variant[size_];
            memcpy(variable_, other.variable_, sizeof(Variant) * size_);
            time_stamp_ = other.time_stamp_;
            time_delay_ = other.time_delay_;
            return *this;
        }
        Event& operator = (Event&& other) {
            name_ = other.name_;
            type_ = other.type_;
            size_ = other.size_;
            variable_ = other.variable_;
            time_stamp_ = other.time_stamp_;
            time_delay_ = other.time_delay_;
            return *this;
        }
        
        // 0. use different channel to handle different event type
        // 1. update & check delay time, decide use event or not
        // 2. dispatch event to listener
        EventType type_;            //  8 bytes
        uint64_t size_ = 0;          //  8 bytes
        EventDataPtr variable_ = nullptr;   //  4 bytes
		double time_stamp_ = 0.0f;  //  ms  4 bytes
        double time_delay_ = 0.0f;  //  ms  4 bytes
        bool handled_ = false;      //  1 byte
        std::string name_;          //  event name

        static ElapseTimer timer_s;
    };
}
```
`Event.cpp`
```
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"

#include <ostream>
#include <sstream>

namespace Rocket {
	ElapseTimer Event::timer_s;

	Event::Event(const std::string& name) 
		: size_(0), variable_(nullptr), name_(name) { 
		type_ = hash(name);
		time_stamp_ = timer_s.GetExactTime();
	}
	Event::Event(const std::string& name, EventDataPtr ptr, uint64_t size) 
		: size_(size), variable_(ptr), name_(name) { 
		type_ = hash(name);
		time_stamp_ = timer_s.GetExactTime();
	}
	Event::Event(const std::string& name, EventType type) 
		: type_(type), size_(0), variable_(nullptr), name_(name) {
		time_stamp_ = timer_s.GetExactTime();
	}
	Event::Event(const std::string& name, EventType type, EventDataPtr ptr, uint64_t size) 
		: type_(type), size_(size), variable_(ptr), name_(name) {
		time_stamp_ = timer_s.GetExactTime();
	}

    std::string Event::ToString() {
        std::stringstream os;
        os << "Name:[" << this->name_ << "]Time:[" << this->time_stamp_ << "]Type:[" << this->type_ << "]Data:{";
		for (int32_t i = 0; i < this->size_; ++i) {
			switch (this->variable_[i].type) {
			case Variant::TYPE_INT32:
				os << "[int32:" << this->variable_[i].as_int32 << "]"; break;
			case Variant::TYPE_UINT32:
				os << "[uint32:" << this->variable_[i].as_uint32 << "]"; break;
			case Variant::TYPE_FLOAT:
				os << "[float:" << this->variable_[i].as_float << "]"; break;
			case Variant::TYPE_DOUBLE:
				os << "[double:" << this->variable_[i].as_double << "]"; break;
			case Variant::TYPE_POINTER:
				os << "[pointer:" << this->variable_[i].as_pointer << "]"; break;
			case Variant::TYPE_STRING_ID:
				os << "[string_id:" << this->variable_[i].as_string_id << "]"; break;
			default:
				RK_ERROR(Event, "Unknow Event Data Type"); break;
			}
		}
		os << "}";
        std::string temp;
        os >> temp;
		return temp;
    }
}
```
## 9.2. 通过Channel实现不同类型事件划分<br>
`EventChannel.h`
```
#pragma once
#include "EventSystem/Event.h"
#include "Containers/Queue/UnboundedQueue.h"
#include "Containers/Queue/BoundedQueue.h"
#include "Utils/TimeStep.h"

#include <vector>

namespace Rocket {
    class EventChannel;
    using ChannelPtr = std::shared_ptr<EventChannel>;
    using EventListener = std::unordered_map<EventType, std::vector<EventDelegate>>;
    using EventStorage = std::unordered_map<EventType, UnboundedQueue<EventPtr>>;

    class EventChannel {
    public:
        explicit EventChannel();                                        // Auto Generate Name
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
        EventListener event_listener_;
        EventStorage event_storage_;
    };
}
```
`EventChannel.cpp`
```
#include "EventSystem/EventChannel.h"

namespace Rocket {
    EventChannel::EventChannel() {
    }

    void EventChannel::RegisterEvent(const EventType& type, const EventDelegate& function) {
        event_listener_[type].push_back(function);
        event_storage_[type].unblock();
        RK_INFO(Event, "Event Listener Size {}", event_listener_[type].size());
        RK_INFO(Event, "Event Queue Size {}", event_storage_.size());
    }

    void EventChannel::UnregisterEvent(const EventType& type, const EventDelegate& function) {
        auto it = event_listener_.find(type);
        if(it == event_listener_.end()) {
            RK_WARN(Event, "Cannot Find Event Delegate");
        }
        else {
            auto delegate = it->second.begin();
            auto end_element = it->second.end();
            while(delegate != end_element) {
                if(*delegate == function) {
                    it->second.erase(delegate);
                    break;
                }
            }
        }
    }

    void EventChannel::Tick(TimeStep step) {
        if(this->IsEmpty()) {
            return;
        }
        else {
            auto event_queue = event_storage_.begin();
            auto end_queue = event_storage_.end();
            while(event_queue != end_queue) {
                EventPtr event;
                auto& queue_ = event_queue->second;
                RK_INFO(Event, "Queue Event Count {}", queue_.size());
                //queue_.block();
                while(queue_.pop(event)) {
                    DispatchEvent(event);
                }
                //queue_.unblock();
                event_queue++;
            }
        }
    }

    void EventChannel::QueueEvent(EventPtr& event) {
        auto event_queue = event_storage_.find(event->GetEventType());
        event_queue->second.push(event);
    }

    void EventChannel::TriggerEvent(EventPtr& event) {
        DispatchEvent(event);
    }

    void EventChannel::DispatchEvent(EventPtr& event) {
        auto delegates = event_listener_.find(event->GetEventType())->second;
        for(auto delegate : delegates) {
            delegate.Invoke(event);
        }
    }
}
```
## 9.3. 实现第一个事件处理<br>
`EventManager.h`
```
#pragma once
#include "Pattern/IRuntimeModule.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventChannel.h"
#include "Utils/Timer.h"

#include <set>
#include <memory>

namespace Rocket {
    //using ChannelMap = std::unordered_map<std::string, ChannelPtr>;
    using ChannelMap = std::unordered_map<uint64_t, ChannelPtr>;
    using EventChannelMap = std::unordered_map<EventType, ChannelMap>;

    class EventManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(EventManager);
    public:
        EventManager() = default;
        virtual ~EventManager() = default;

        [[nodiscard]] virtual int Initialize() final;
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

    // Must Generate An Instance Somewhere
    extern EventManager* g_EventManager;
}

#define REGISTER_DELEGATE_CLASS(class,function,instance,name,ch_name) {\
    Rocket::EventDelegate delegate; \
    delegate.Bind<class,&function>(instance); \
    Rocket::g_EventManager->AddEventListener(delegate, name, ch_name); }
#define REGISTER_DELEGATE_FN(function,name,ch_name) {\
    Rocket::EventDelegate delegate; \
    delegate.Bind<&function>(); \
    Rocket::g_EventManager->AddEventListener(delegate, name, ch_name); }
```
`EventManager.cpp`
```
#include "EventSystem/EventManager.h"
#include "Log/Log.h"

namespace Rocket {
    int EventManager::Initialize() {
        Event::timer_s.Start();
        channels_.clear();
        event_channel_map_.clear();
        return 0;
    }

    void EventManager::Finalize() {
        Event::timer_s.Stop();
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
        auto result = event_channel_map_.find(type);
        for(auto channel : result->second) {
            channel.second->QueueEvent(event);
        }
    }

    void EventManager::TriggerEvent(EventPtr& event) {
        EventType type = event->GetEventType();
        auto result = event_channel_map_.find(type);
        for(auto channel : result->second) {
            channel.second->TriggerEvent(event);
        }
    }

    void EventManager::AddChannel(const std::string& name, ChannelPtr channel) {
        event_channel_map_[hash(name)][hash(channel->GetName())] = channel;
        channels_[hash(channel->GetName())] = channel;
        RK_INFO(Event, "Channels Size {}", channels_.size());
    }

    void EventManager::AddChannel(EventType type, ChannelPtr channel) {
        event_channel_map_[type][hash(channel->GetName())] = channel;
        channels_[hash(channel->GetName())] = channel;
        RK_INFO(Event, "Channels Size {}", channels_.size());
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
```
使用事件系统进行多线程事件处理<br>
`event_test.cpp`
```
#include "Memory/MemoryCheck.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventChannel.h"
#include "EventSystem/EventManager.h"
#include "Log/Log.h"

#include <random>
#include <thread>
#include <mutex>
#include <chrono>

using namespace Rocket;

namespace Rocket {
    EventManager* g_EventManager = nullptr;
}

bool EventHandle(EventPtr& event) {
    RK_TRACE(Event, "EventHandle: {}", event->ToString());
    return false;
}

struct TestEventHandle {
    bool EventHandle(EventPtr& event) {
        RK_TRACE(Event, "EventHandleClass: {}", event->ToString());
        return false;
    }
};

int main() {
    Rocket::Log::Init();
    // Initialize
    g_EventManager = new EventManager;
    auto result = g_EventManager->Initialize();
    if(result != 0) {
        return 1;
    }
    ChannelPtr channel = std::shared_ptr<EventChannel>(new EventChannel("test_channel"));
    g_EventManager->AddChannel("test", channel);
    // Event Listener Register
    REGISTER_DELEGATE_FN(EventHandle, "test", "test_channel");
    TestEventHandle test;
    REGISTER_DELEGATE_CLASS(TestEventHandle, TestEventHandle::EventHandle, &test, "test", "test_channel");
    // Event Generate
    EventPtr event = EventPtr(new Event("test"));
    RK_TRACE(Event, "{}", event->ToString());
    // Event Dispatch
    [[maybe_unused]] auto consumer_function = [&]() {
		while(1) {
            for(int i = 0; i < 100; ++i) {
                Variant* data = new Variant[4];
                for(int j = 0; j < 4; ++j) {
                    data[j].type = Variant::TYPE_INT32;
                    data[j].as_int32 = std::rand();
                }
                EventPtr event = EventPtr(new Event("test", data, 4));
                g_EventManager->QueueEvent(event);
            }
			g_EventManager->Tick(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	};

    [[maybe_unused]] auto produce_function = [&](){
        while(1) {
            Variant* data = new Variant[4];
            for(int j = 0; j < 4; ++j) {
                data[j].type = Variant::TYPE_INT32;
                data[j].as_int32 = std::rand();
            }
            EventPtr event = EventPtr(new Event("test", data, 4));
            g_EventManager->QueueEvent(event);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    };

    std::thread consumer([&]() {
		while(1) {
            for(int i = 0; i < 100; ++i) {
                Variant* data = new Variant[4];
                for(int j = 0; j < 4; ++j) {
                    data[j].type = Variant::TYPE_INT32;
                    data[j].as_int32 = std::rand();
                }
                EventPtr event = EventPtr(new Event("test", data, 4));
                g_EventManager->QueueEvent(event);
            }
			g_EventManager->Tick(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});

    std::thread producer_1(produce_function);
    std::thread producer_2(produce_function);
    std::thread producer_3(produce_function);
    std::thread producer_4(produce_function);

    producer_1.join();
    producer_2.join();
    producer_3.join();
    producer_4.join();
	consumer.join();

    // Finalize
    g_EventManager->Finalize();
    delete g_EventManager;

    return 0;
}
```