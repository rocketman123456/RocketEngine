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
    std::thread consumer([&]() {
		while(1) {
			g_EventManager->Tick(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});

    auto produce_function = [&](){
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