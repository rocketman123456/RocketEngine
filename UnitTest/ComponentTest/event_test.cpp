#include "EventSystem/Event.h"
#include "Log/Log.h"

#include <random>

using namespace Rocket;


int main() {
    Rocket::Log::Init();

    EventVarVec var;
    for(int i = 0; i < 10; i++)
        var.emplace_back(Variant::TYPE_INT32, std::rand());
    Event event(std::move(var));

    RK_TRACE(Event, "Event {}", event.ToString());

    return 0;
}