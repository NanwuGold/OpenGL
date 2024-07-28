#include "Buffer.h"

#include <RenderBase/OpenGL/OpenGLBuffer.h>
#include <RenderBase/OpenGL/OpenGLUniformBuffer.h>

namespace OBase
{
    Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
    {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        return std::make_shared<OpenGLVertexBuffer>(size);
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
    {
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint8_t bindPoint)
    {
        return OBase::CreateRef<OpenGLUniformBuffer>(size,bindPoint);
    }
}
