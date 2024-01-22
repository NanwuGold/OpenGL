#include "OpenGLVertexArray.h"
#include "Buffer.h"
#include <glad/glad.h>

namespace FXAA
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

        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
        m_VertexBuffers.clear();
        m_IndexBuffer.reset();
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::UnBind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        auto &layout = vertexBuffer->GetLayout();
        for (const auto &element: layout)
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

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }

    const std::vector<Ref<VertexBuffer>> &OpenGLVertexArray::GetVertexBuffers() const
    {
        return m_VertexBuffers;
    }

    const Ref<IndexBuffer> &OpenGLVertexArray::GetIndexBuffer() const
    {
        return m_IndexBuffer;
    }
}
