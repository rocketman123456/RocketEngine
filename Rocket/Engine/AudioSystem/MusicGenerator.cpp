#include "AudioSystem/MusicGenerator.h"

#include <iostream>

namespace Rocket {
    int32_t MusicGenerator::Initialize(JsonParserPtr&& ptr) {
        json_file_ = std::move(ptr);
        auto raw = json_file_->GetRawJson();

        base_ = (*raw)["base"];
        type_ = (*raw)["type"];
        one_step_ = (*raw)["one_step"];

        auto music = (*raw)["music"];
        for(auto it = music.begin(); it != music.end(); ++it) {
            std::cout << it.key() << " : " << it.value() << "\n";
        }

        return 0;
    }
}