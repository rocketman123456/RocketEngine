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

        base_ = (raw)["base"].string_value();
        type_ = (raw)["type"].string_value();
        one_step_ = (raw)["one_step"].int_value();

        events.clear();
        static std::unordered_map<std::string, double> start_map;
        static std::unordered_map<std::string, double> last_map;

        auto music = (raw)["music"].array_items();
        for(auto note = music.begin(); note != music.end(); ++note) {
            std::cout << "{" << std::endl;
            std::cout << "  id : " << (*note)["id"].string_value() << "\n";
            std::cout << "  note : " << (*note)["note"].string_value() << "\n";
            std::cout << "  begin : " << (*note)["begin"].number_value() << "\n";
            std::cout << "  delay : " << (*note)["delay"].number_value() << "\n";
            std::cout << "  last : " << (*note)["last"].number_value() << "\n";
            std::cout << "}" << std::endl;

            double start_time = 0;
            if(!(*note)["begin"].is_null()) {
                std::string begin_name = (*note)["begin"].string_value();
                auto temp_event_start = start_map[begin_name];
                auto temp_note_last = last_map[begin_name];
                start_time = temp_event_start + temp_note_last * one_step_;
            }
            double time_delay = (*note)["delay"].number_value();
            double time_last = (*note)["last"].number_value();

            if((*note)["note"].is_null()) {
                continue;
            }

            std::string note_name = (*note)["note"].string_value();
            std::string complete_name = base_ + note_name + type_;
            Variant* data = new Variant[1];
            data[0].type = Variant::TYPE_STRING_ID;
            data[0].as_string_id = hash(complete_name);
            EventPtr event = EventPtr(new Event("audio", data, 1));
            event->time_delay_ = start_time + time_delay * one_step_;
            events.push_back(event);

            std::string id = (*note)["id"].string_value();
            start_map[id] = event->time_delay_;
            last_map[id] = time_last;
        }
        start_map.clear();
        last_map.clear();
    }
}
