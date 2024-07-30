#include "OpenGLUniformBuffer.h"
#include <glad/glad.h>

namespace OBase
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformLayout& layout, const uint8_t bindPoint)
        : m_RenderID(0)
        , m_Size(layout.GetStride())
        , m_BindPoint(bindPoint)
        , m_UniformLayout(layout)
    {
        invaild();
    }

    void OpenGLUniformBuffer::Bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_RenderID);
    }

    void OpenGLUniformBuffer::UnBind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUniformBuffer::invaild()
    {
        glCreateBuffers(1, &m_RenderID);
        glNamedBufferData(m_RenderID, m_Size, nullptr, GL_STATIC_DRAW);
        // glBufferData(GL_UNIFORM_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    }

    void OpenGLUniformBuffer::UpdateData(const uint32_t offset, const uint32_t size, const void *data)
    {
        glNamedBufferSubData(m_RenderID, offset, size, data);
    }

    void OpenGLUniformBuffer::LinkBindingPoint()
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_BindPoint, m_RenderID);
        // OR
        // glBindBufferRange(GL_UNIFORM_BUFFER, m_BindPoint, m_RenderID, 0, m_Size);
    }

    uint8_t OpenGLUniformBuffer::BindPoint()
    {
        return m_BindPoint;
    }

    UniformLayout& OpenGLUniformBuffer::GetLayout()
    {
        return m_UniformLayout;
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glDeleteBuffers(1, &m_RenderID);
    }

    void OpenGLUniformBuffer::UpdateElementData(const std::string &name, const void *data)
    {
        auto elements = m_UniformLayout.GetElements();
        auto item = std::find_if(elements.begin(),elements.end(),[name](UniformLayoutElement & element){
            return element.m_Name == name;
        });
        if(item != elements.end())
        {
            UpdateData(item->m_Offset, item->m_Size, data);
        }
    }


} // OBase