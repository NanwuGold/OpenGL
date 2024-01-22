#include "OpenGLVertexArray.h"
#include <Hazel/Renderer/Buffer.h>
#include <glad/glad.h>

namespace Hazel
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;

        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;

        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;

        case ShaderDataType::Bool:
            return GL_BOOL;
        case ShaderDataType::None:
            break;
        }

        HZ_CORE_ASSERT(false, "UnKnown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        HZ_PROFILE_FUNCTION()

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        HZ_PROFILE_FUNCTION()

        glDeleteVertexArrays(1, &m_RendererID);
        m_VertexBuffers.clear();
        m_IndexBuffer.reset();
    }

    void OpenGLVertexArray::Bind() const
    {
        HZ_PROFILE_FUNCTION()

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::UnBind() const
    {
        HZ_PROFILE_FUNCTION()

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {
        HZ_PROFILE_FUNCTION()

        HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        auto &layout = vertexBuffer->GetLayout();
        for (const auto &element : layout)
        {
            glEnableVertexAttribArray(index);

            glVertexAttribPointer(index,
                                  static_cast<GLint>(element.GetComponentCount()),
                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                  element.Normalized ? GL_TRUE : GL_FALSE,
                                  static_cast<GLint>(layout.GetStride()),
                                  reinterpret_cast<const void *>(element.Offset));

            index++;
        }

        m_VertexBuffers.emplace_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexbuffer)
    {
        HZ_PROFILE_FUNCTION()

        glBindVertexArray(m_RendererID);
        indexbuffer->Bind();

        m_IndexBuffer = indexbuffer;
    }

    const std::vector<Ref<VertexBuffer>> &OpenGLVertexArray::GetVertexBuffers() const
    {
        HZ_PROFILE_FUNCTION()

        return m_VertexBuffers;
    }

    const Ref<IndexBuffer> &OpenGLVertexArray::GetIndexBuffer() const
    {
        HZ_PROFILE_FUNCTION()

        return m_IndexBuffer;
    }

} // Hazel
