#pragma once

namespace Rocket {
    class Uncopyable {
    public:
        Uncopyable() = default;
        virtual ~Uncopyable() = default;
    private:
        Uncopyable(const Uncopyable&);
        Uncopyable& operator=(const Uncopyable&);
    };
}
