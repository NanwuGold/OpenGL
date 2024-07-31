#ifndef OPENGL_UNIFORM_BUFFER_H_
#define OPENGL_UNIFORM_BUFFER_H_


#include <stdexcept>
#include <string>
#include <RenderBase/pointer_ptr.hpp>

namespace OBase
{
    enum class ElementDataType
    {
        None = 0,
        Float,    ///< 4
        Float2,   ///< 32  单个元素与vec4相同
        Float3,   ///< 48  单个元素与vec4相同
        Float4,   ///< 64  单个元素与vec4相同
        Int,      ///< 4
        Bool,     ///< 4
        Matrix,   ///< 16 * 4 = 64
        Vec2,     ///< 4 * 2
        Vec3,     ///< 4 * 4 = 16
        Vec4      ///< 16
    };


    /**
     * @brief 用于描述uniform buffer中的每一个元素
     * @note 请按照std140的布局方式设置内容
     */
    struct UniformLayoutElement
    {
        std::string m_Name{};
        ElementDataType m_DataType{ElementDataType::None};
        uint32_t m_Size{};
        uint32_t m_Offset{};

        UniformLayoutElement(const ElementDataType& type, std::string name)
        : m_Name(std::move(name))
        , m_DataType(type)
        {
            
        }

        uint32_t GetDataComponentSize() const
        {
            switch (m_DataType)
            {
            case ElementDataType::Float:
                return 4;
            case ElementDataType::Float2:
                return 32;
            case ElementDataType::Float3:
                return 48;
            case ElementDataType::Float4:
                return 64;
            case ElementDataType::Int:
            case ElementDataType::Bool:
                return 4;
            case ElementDataType::Matrix:
                return 64;
            case ElementDataType::Vec2:
                return 8;
            case ElementDataType::Vec3:
            case ElementDataType::Vec4:
                return 16;
            case ElementDataType::None:
                return 0;
            default: 
                throw std::runtime_error("UnKnow Type!");
            }
        }
    };

    class UniformLayout
    {
    public:
        UniformLayout(const std::initializer_list<UniformLayoutElement> & elements)
        : m_Elements(elements)
        , m_Stride(0)
        {
            CalculateOffsetsAndStride();
        }

        UniformLayout(const UniformLayout&) = default;
        UniformLayout(UniformLayout &&) = delete;

        UniformLayout& operator=(const UniformLayout&) = default;
        UniformLayout& operator=(UniformLayout&&) = delete;

        virtual ~UniformLayout() { m_Elements.clear(); }

        const std::vector<UniformLayoutElement>& GetElements();
        uint32_t GetStride() const;

    private:
        void CalculateOffsetsAndStride();

    private:
        std::vector<UniformLayoutElement> m_Elements;
        uint32_t m_Stride;

    };

    class UniformBuffer
    {
    public:
        UniformBuffer() = default;
        virtual ~UniformBuffer() = default;

        UniformBuffer& operator=(const UniformBuffer&) = delete;
        UniformBuffer& operator=(UniformBuffer&&) = delete;

        UniformBuffer(UniformBuffer&&) = delete;
        UniformBuffer(UniformBuffer&) = delete;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void UpdateElementData(const std::string & name, const void * data) = 0;

        virtual void LinkBindingPoint() = 0;
        virtual uint8_t BindPoint() = 0;

        virtual UniformLayout& GetLayout() = 0;

        static Ref<UniformBuffer> Create(const UniformLayout & layout, uint8_t bindPoint);
    };

}
#endif // !OPENGL_UNIFORM_BUFFER_H_
