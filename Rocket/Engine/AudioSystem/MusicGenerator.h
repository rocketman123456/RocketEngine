#pragma once
#include "Parser/JsonParser.h"
#include "EventSystem/Event.h"

#include <vector>

namespace Rocket {
    class MusicGenerator {
    public:
        int32_t Initialize(JsonParserPtr&& ptr);
        std::vector<EventPtr>& GetMusicNotes() { return events_; }
    private:
        JsonParserPtr json_file_;
        std::vector<EventPtr> events_;
        std::string type_;
        std::string base_;
        double one_step_;
    };
}