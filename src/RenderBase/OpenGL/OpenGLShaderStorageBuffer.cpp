#include "OpenGLShaderStorageBuffer.h"

#include <cassert>
#include <glad/glad.h>

namespace OBase
{
    Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(uint32_t size)
    {
        return OBase::CreateRef<OpenGLShaderStorageBuffer>(size, 0);
    }

    void OpenGLShaderStorageBuffer::Bind() const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RenderID);
    }

    void OpenGLShaderStorageBuffer::UnBind()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void OpenGLShaderStorageBuffer::ReLinkBindingPoint(const uint8_t bindPoint)
    {
        m_BindPoint = bindPoint;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindPoint, m_RenderID);
    }

    uint8_t OpenGLShaderStorageBuffer::BindPoint() const
    {
        return m_BindPoint;
    }

    unsigned OpenGLShaderStorageBuffer::RenderID()
    {
        return m_RenderID;
    }

    void OpenGLShaderStorageBuffer::UpdateData(const uint32_t offset, const uint32_t size, const void *data)
    {
        assert(offset + size <= m_Size && "Update range exceeds buffer size!");
        glNamedBufferSubData(m_RenderID, offset, size, data);
    }

    OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const uint32_t size, uint8_t bindPoint)
        : m_Size(size), m_RenderID(0), m_BindPoint(bindPoint)
    {
        Invalidate();
    }

    OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
    {
        if (glIsBuffer(m_RenderID))
        {
            glDeleteBuffers(1, &m_RenderID);
            m_RenderID = 0;
        }
    }

    void OpenGLShaderStorageBuffer::Invalidate()
    {
        if (glIsBuffer(m_RenderID))
        {
            glDeleteBuffers(1, &m_RenderID);
        }
        glCreateBuffers(1, &m_RenderID);
        glNamedBufferData(m_RenderID, m_Size, nullptr, GL_DYNAMIC_DRAW);

        ReLinkBindingPoint(0); ///< 绑定到默认的绑定点 0
    }
}