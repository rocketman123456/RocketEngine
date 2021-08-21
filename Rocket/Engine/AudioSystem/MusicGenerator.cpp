#include "AudioSystem/MusicGenerator.h"

#include <iostream>
#include <string>

namespace Rocket {
    int32_t MusicGenerator::Initialize(JsonParserPtr&& ptr) {
        json_file_ = std::move(ptr);
        return 0;
    }

    void MusicGenerator::Finalize() {
        json_file_.reset();
    }   

    void MusicGenerator::GetMusicNotes(std::vector<EventPtr>& events) {
        auto raw = json_file_->GetRawJson();

        base_ = (*raw)["base"];
        type_ = (*raw)["type"];
        one_step_ = (*raw)["one_step"];

        events.clear();
        std::unordered_map<std::string, EventPtr> event_map;

        auto music = (*raw)["music"];
        for(auto note = music.begin(); note != music.end(); ++note) {
            // std::cout << "id : " << (*note)["id"] << "\n";
            // std::cout << "note : " << (*note)["note"] << "\n";
            // std::cout << "begin : " << (*note)["begin"] << "\n";
            // std::cout << "delay : " << (*note)["delay"] << "\n";
            // std::cout << "last : " << (*note)["last"] << "\n";

            std::string note_name = (*note)["note"];
            std::string complete_name = base_ + note_name + type_;
            Variant* data = new Variant[1];
            data[0].type = Variant::TYPE_STRING_ID;
            data[0].as_string_id = hash(complete_name);
            EventPtr event = EventPtr(new Event("audio", data, 1));

            double start_time = 0;
            if(!(*note)["begin"].is_null()) {
                std::string begin_name = (*note)["begin"];
                auto temp_event = event_map[begin_name];
                start_time = temp_event->time_delay_;
            }

            start_time += one_step_ * (double)(*note)["delay"];
            event->time_delay_ = start_time;

            std::string id = (*note)["id"];
            event_map[id] = event;
            events.push_back(event);
        }
    }
}