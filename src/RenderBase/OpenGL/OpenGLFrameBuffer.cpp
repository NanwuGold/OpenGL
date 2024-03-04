#include "OpenGLFrameBuffer.h"

#include <iostream>

OpenGLFrameBuffer::OpenGLFrameBuffer()
    :m_RendererID(0)
{
}

void OpenGLFrameBuffer::Create(std::vector<OBase::Ref<Texture>> colorAttachments, OBase::Ref<Texture> depth)
{
    m_colorAttachments.clear();
    m_colorAttachments.insert(m_colorAttachments.end(),colorAttachments.begin(), colorAttachments.end());
    m_depthAttachment = depth;

    Invalidate();
}

void OpenGLFrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER,m_RendererID);
}

void OpenGLFrameBuffer::Resize(int w, int h)
{

}

void OpenGLFrameBuffer::Invalidate()
{
    if(m_RendererID != 0)
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }
    
    glGenFramebuffers(1, &m_RendererID);
    Bind();

    auto index = 0;
    std::vector<GLenum> drawBuffers;
    auto target = GL_TEXTURE_2D;
    for(const auto& texture: m_colorAttachments)
    {
        drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + index);

        if(texture->multiSampleFlag())
        {
            target = GL_TEXTURE_2D_MULTISAMPLE;
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers.back(), target, texture->RenderID(), 0);
        index++;
    }

    GLenum attachment = GL_DEPTH_STENCIL_ATTACHMENT;
    switch (m_depthAttachment->format())
    {
        case GL_DEPTH24_STENCIL8:
        case  GL_DEPTH32F_STENCIL8:
            attachment = GL_DEPTH_STENCIL_ATTACHMENT;
            break;
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            attachment = GL_DEPTH_ATTACHMENT;
            break;
        default: 
            throw std::runtime_error("Format error!");
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER,attachment, target, m_depthAttachment->RenderID(), 0);
    glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);

}

void OpenGLFrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned OpenGLFrameBuffer::RenderID()
{
    return m_RendererID;
}
