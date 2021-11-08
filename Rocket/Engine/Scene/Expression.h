#pragma once

namespace Rocket {
    class Expression {
    public:
        virtual ~Expression() {}
        virtual double Evaluate() = 0;
    };
}
