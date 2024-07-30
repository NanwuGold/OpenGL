#include "UniformBuffer.h"
#include <RenderBase/OpenGL/OpenGLUniformBuffer.h>

namespace OBase
{
    Ref<UniformBuffer> UniformBuffer::Create(const UniformLayout& layout, uint8_t bindPoint)
    {
        return OBase::CreateRef<OpenGLUniformBuffer>(layout, bindPoint);
    }

    const std::vector<UniformLayoutElement>& UniformLayout::GetElements()
    {
        return m_Elements;
    }

    uint32_t UniformLayout::GetStride() const
    {
        return m_Stride;
    }

    void UniformLayout::CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        for (auto& element : m_Elements)
        {
            element.m_Offset = offset;
            element.m_Size = element.GetDataComponentSize();
            offset += element.m_Size;
        }
        m_Stride = offset;
    }
}