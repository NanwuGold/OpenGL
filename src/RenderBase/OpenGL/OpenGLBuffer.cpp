#include "OpenGLBuffer.h"
#include <glad/glad.h>
namespace OBase
{
    /////////////////////////////////////////////////////////////////////////
    // VertexBuffer /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
            : m_RendererID(0)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
            : m_RendererID(0)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::UnBind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetLayout(const BufferLayout &layout)
    {
        m_Layout = layout;
    }

    const BufferLayout &OpenGLVertexBuffer::GetLayout() const
    {
        return m_Layout;
    }

    void OpenGLVertexBuffer::SetDate(void *data, uint32_t size)
    {
        /// TODO : need update
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        GLint bufferSize = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        void *bufferData = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        std::memset(bufferData, 0, bufferSize);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    /////////////////////////////////////////////////////////////////////////
    // IndexBuffer //////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
            : m_Count(count), m_RendererID(0)
    {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(uint32_t) * count), indices,
                     GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::UnBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::GetCount()
    {
        return m_Count;
    }
}
