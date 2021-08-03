#include "EventSystem/Event.h"
#include "Log/Log.h"

#include <random>

using namespace Rocket;


int main() {
    Rocket::Log::Init();

    {
        // Event Log
        Event event("test", 0x01);
        RK_TRACE(Event, "{}", event.ToString());
    }

    {
        // Event Listener Register
        // Event Generate
        // Event Dispatch
    }

    {
        // Performance Test
        // Dispatch 100,000 Event
    }

    return 0;
}