#pragma once
#include "Log/Log.h"

#include <memory>
#include <cstdint>
#include <string>
#include <initializer_list>
#include <vector>

namespace Rocket {
    enum class BufferType : uint32_t {
        COLOR = 0,
        DEPTH,
        UNKNOWN,
    };

    enum class BufferDataType : uint32_t {
        NONE = 0,
        FLOAT, FLOAT2, FLOAT3, FLOAT4,
        INT, INT2, INT3, INT4,
        MAT2, MAT3, MAT4,
        BOOL,
    };

    static uint32_t BufferDataTypeSize(BufferDataType type) {
		switch (type) {
			case BufferDataType::FLOAT:    return 4;
			case BufferDataType::FLOAT2:   return 4 * 2;
			case BufferDataType::FLOAT3:   return 4 * 3;
			case BufferDataType::FLOAT4:   return 4 * 4;
            case BufferDataType::MAT2:     return 4 * 2 * 2;
			case BufferDataType::MAT3:     return 4 * 3 * 3;
			case BufferDataType::MAT4:     return 4 * 4 * 4;
			case BufferDataType::INT:      return 4;
			case BufferDataType::INT2:     return 4 * 2;
			case BufferDataType::INT3:     return 4 * 3;
			case BufferDataType::INT4:     return 4 * 4;
			case BufferDataType::BOOL:     return 1;
            default: RK_ERROR(Render, "Unknown BufferDataType"); return 0;
		}
	}

    static uint32_t BufferDataTypeCount(BufferDataType type) {
		switch (type) {
			case BufferDataType::FLOAT:    return 1;
			case BufferDataType::FLOAT2:   return 2;
			case BufferDataType::FLOAT3:   return 3;
			case BufferDataType::FLOAT4:   return 4;
            case BufferDataType::MAT2:     return 2;    // * float2
			case BufferDataType::MAT3:     return 3;    // * float3
			case BufferDataType::MAT4:     return 4;    // * float4
			case BufferDataType::INT:      return 1;
			case BufferDataType::INT2:     return 2;
			case BufferDataType::INT3:     return 3;
			case BufferDataType::INT4:     return 4;
			case BufferDataType::BOOL:     return 1;
            default: RK_ERROR(Render, "Unknown BufferDataType"); return 0;
		}
	}

    typedef struct BufferElement {
        std::string name = "";
		BufferDataType type = BufferDataType::NONE;
		uint32_t size = 0;
		size_t offset = 0;
		bool normalized = false;

        BufferElement() = default;
        ~BufferElement() = default;

        BufferElement(BufferDataType type, const std::string& name, bool normalized = false)
			: name(name), type(type), size(BufferDataTypeSize(type)), normalized(normalized) {}
    } BufferElement;

    class BufferLayout {
    public:
        BufferLayout() = default;
        ~BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> elements) : elements_(elements) {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return stride_; }
		const std::vector<BufferElement>& GetElements() const { return elements_; }

		std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
		std::vector<BufferElement>::iterator end() { return elements_.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }
    private:
        void CalculateOffsetsAndStride() {
			size_t offset = 0;
			stride_ = 0;
			for (auto& element : elements_) {
				element.offset = offset;
				offset += element.size;
				stride_ += element.size;
			}
		}
    private:
        std::vector<BufferElement> elements_;
        uint32_t stride_ = 0;
    };

    class RenderBuffer {
    public:
        virtual ~RenderBuffer() = default;

        virtual void SetBufferLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetBufferLayout() const = 0;
    };

    using RenderBufferPtr = std::unique_ptr<RenderBuffer>;
}
