#pragma once
#include "Core/Core.h"
#include "Utils/TimeStep.h"

namespace Rocket {
    Interface IRuntimeModule {
    public:
        IRuntimeModule() {}
        virtual ~IRuntimeModule() = default;

        [[nodiscard]] virtual int32_t Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick(TimeStep ts) = 0;
        // For Debug
        [[nodiscard]] virtual String ToString() const { return GetName(); }
    protected:
        [[nodiscard]] virtual const char* GetName() const = 0;
    };

    // Every Module should Declare This
#define RUNTIME_MODULE_TYPE(type) \
    virtual const char* GetName() const override { return #type; }

    inline std::ostream &operator << (std::ostream& os, const IRuntimeModule& r) {
		return os << r.ToString();
	}
}