#include "Memory/MemoryCheck.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventChannel.h"
#include "EventSystem/EventManager.h"
#include "Log/Log.h"

#include <random>

using namespace Rocket;

namespace Rocket {
    EventManager* g_EventManager = nullptr;
}

bool EventHandle(EventPtr& event) {
    return false;
}

struct TestEventHandle {
    bool EventHandle(EventPtr& event) {
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
    g_EventManager->AddChannel("test"_hash, channel);
    // Event Listener Register
    REGISTER_DELEGATE_FN(EventHandle, "test", "test_channel");
    TestEventHandle test;
    REGISTER_DELEGATE_CLASS(TestEventHandle, TestEventHandle::EventHandle, &test, "test", "test_channel");
    // Event Generate
    EventPtr event = EventPtr(new Event("test"));
    RK_TRACE(Event, "{}", event->ToString());
    // Event Dispatch
    for(int i = 0; i < 100'000; ++i) {
        g_EventManager->Tick(10);
        for(int k = 0; k < 100'000; ++k){
            Variant* data = new Variant[20];
            for(int j = 0; j < 20; ++j) {
                data[j].type = Variant::TYPE_INT32;
                data[j].as_int32 = std::rand();
            }
            EventPtr event = EventPtr(new Event("test", data, 20));
            g_EventManager->QueueEvent(event);
        }
    }
    // Finalize
    g_EventManager->Finalize();
    delete g_EventManager;

    return 0;
}