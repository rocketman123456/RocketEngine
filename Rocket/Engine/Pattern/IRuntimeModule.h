#pragma once
#include "Core/Declare.h"
#include "Utils/TimeStep.h"

#include <string>
#include <ostream>

namespace Rocket {
    Interface IRuntimeModule {
    public:
        virtual ~IRuntimeModule() = default;

        [[nodiscard]] virtual int Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick(TimeStep step) = 0;
        // For Debug
        [[nodiscard]] virtual inline std::string ToString() const { return GetName(); }
    protected:
        [[nodiscard]] virtual inline const char* GetName() const = 0;
    };

    inline std::ostream& operator << (std::ostream& os, const IRuntimeModule& r) {
		return os << r.ToString();
	}
}

#define RUNTIME_MODULE_TYPE(type) \
    virtual const char* GetName() const override { return #type; }