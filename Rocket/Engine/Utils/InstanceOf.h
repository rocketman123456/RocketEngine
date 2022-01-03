#pragma once

namespace Rocket {
    // template<typename Base, typename T>
    // inline bool instanceof(const T*) {
    //     return std::is_base_of<Base, T>::value;
    // }

    template<typename Base, typename T>
    inline bool instanceof(const T *ptr) {
        return dynamic_cast<const Base*>(ptr) != nullptr;
    }
}
