#pragma once
#include "Core/Declare.h"
#include "Pattern/Singleton.h"

#include <memory>
#include <cstdint>

namespace Rocket {
    _Interface_ Command {
    public:
        virtual ~Command() = default;
        virtual void Execution() = 0;
        virtual void Revocation() = 0;
    protected:
        int32_t id_ = 0;
    };

    _Interface_ CommandStatic : _implements_ Command { // , implements AbstractSingleton<CommandStatic>
    public:
        virtual ~CommandStatic() = default;
        virtual std::shared_ptr<CommandStatic> GetCommand() = 0;
    };

    // std::shared_ptr<D> d = std::dynamic_pointer_cast<D>(d_child);
    using CommandPtr = std::shared_ptr<Command>;
    using CommandStaticPtr = std::shared_ptr<CommandStatic>;
}
