#pragma once

#include <cstdint>

namespace Rocket {
    enum class RenderPrimitive : int32_t {
		LINE = 0,
		TRIANGLE,
	};

    enum class ShaderType : uint32_t {
        VERTEX_SHADER = 0,
        FRAGMENT_SHADER,
        COMPUTE_SHADER,
        UNKNOWN_SHADER,
    };

    enum class BufferType : int32_t {
        COLOR = 1,
        DEPTH = 2,
        UNKNOWN,
    };

	inline BufferType operator | (BufferType a, BufferType b) {
		return BufferType((int)a | (int)b);
	}

	inline BufferType operator & (BufferType a, BufferType b) {
		return BufferType((int)a & (int)b);
	}

    enum class BufferDataType : uint32_t {
        NONE = 0,
        FLOAT, FLOAT2, FLOAT3, FLOAT4,
        INT, INT2, INT3, INT4,
        MAT2, MAT3, MAT4,
        BOOL,
    };
}
