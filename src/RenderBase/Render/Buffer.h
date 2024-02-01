#ifndef OPENGL_BUFFER_H_
#define OPENGL_BUFFER_H_

#include <memory>
#include <utility>
#include <string>
#include <vector>

#include "RenderBase/pointer_ptr.hpp"

namespace OBase
{
    enum class ShaderDataType : int
    {
        None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:
                return 4 * 1;
            case ShaderDataType::Float2:
                return 4 * 2;
            case ShaderDataType::Float3:
                return 4 * 3;
            case ShaderDataType::Float4:
                return 4 * 4;

            case ShaderDataType::Mat3:
                return 4 * 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;

            case ShaderDataType::Int:
                return 4 * 1;
            case ShaderDataType::Int2:
                return 4 * 2;
            case ShaderDataType::Int3:
                return 4 * 3;
            case ShaderDataType::Int4:
                return 4 * 4;

            case ShaderDataType::Bool:
                return 1 * 1;
            case ShaderDataType::None:
                break;
        }

        return 0;
    }

    struct BufferElement
    {
        std::string Name;                           ///< 数据的名称
        ShaderDataType Type{ShaderDataType::None};  ///< 数据的类型
        uint32_t Size{};    ///< 数据的大小
        uint32_t Offset{};  ///< 偏移
        bool Normalized{};  ///< 是否归一化

        BufferElement() = default;

        BufferElement(ShaderDataType type, std::string name, bool normalized = false)
                : Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
                  Normalized(normalized)
        {
        }

        [[nodiscard]] uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case ShaderDataType::Float:
                    return 1;
                case ShaderDataType::Float2:
                    return 2;
                case ShaderDataType::Float3:
                    return 3;
                case ShaderDataType::Float4:
                    return 4;

                case ShaderDataType::Mat3:
                    return 3 * 3;
                case ShaderDataType::Mat4:
                    return 4 * 4;

                case ShaderDataType::Int:
                    return 1;
                case ShaderDataType::Int2:
                    return 2;
                case ShaderDataType::Int3:
                    return 3;
                case ShaderDataType::Int4:
                    return 4;

                case ShaderDataType::Bool:
                    return 1;

                case ShaderDataType::None:
                    break;
            }

            return 0;
        }


    };

    class BufferLayout
    {
    public:

        BufferLayout(const std::initializer_list<BufferElement> &elements)
                : m_Elements(elements)
        {
            CalculateOffsetAndStride();
        }

        [[maybe_unused]] [[nodiscard]] const std::vector<BufferElement> &GetElements() const
        { return m_Elements; }

        [[nodiscard]] uint32_t GetStride() const
        { return m_Stride; }

        std::vector<BufferElement>::iterator begin()
        { return m_Elements.begin(); }

        std::vector<BufferElement>::iterator end()
        { return m_Elements.end(); }

        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const
        { return m_Elements.begin(); }

        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const
        { return m_Elements.end(); }

    private:
        void CalculateOffsetAndStride()
        {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto &element: m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;

    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void UnBind() const = 0;

        virtual void SetDate(void *data, uint32_t size) = 0;

        [[maybe_unused]] virtual void SetLayout(const BufferLayout &layout) = 0;

        [[maybe_unused]] [[nodiscard]] virtual const BufferLayout &GetLayout() const = 0;

        static Ref<VertexBuffer> Create(float *vertices, uint32_t size);

        static Ref<VertexBuffer> Create(uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;

        [[maybe_unused]] virtual void UnBind() const = 0;

        [[maybe_unused]] virtual uint32_t GetCount() = 0;

        static Ref<IndexBuffer> Create(uint32_t *indices, uint32_t count);
    };
}

#endif //! OPENGL_BUFFER_H_

