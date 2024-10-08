#include "OpenGLFrameBuffer.h"

#include <iostream>
#include <RenderBase/Core/Core.h>

namespace OBase
{
    OpenGLFrameBuffer::~OpenGLFrameBuffer() = default;

    OpenGLFrameBuffer::OpenGLFrameBuffer()
        : FrameBuffer()
          , m_RendererID(0)
    {
    }

    void OpenGLFrameBuffer::Create(std::vector<Ref<Texture>> colorAttachments, Ref<Texture> depth)
    {
        m_colorAttachments.clear();
        m_colorAttachments.insert(m_colorAttachments.end(), colorAttachments.begin(), colorAttachments.end());

        m_Attachments.insert({FramebufferAttachment::Depth, depth});

        auto index = static_cast<uint32_t>(FramebufferAttachment::Color0);
        for (auto& attachment : colorAttachments)
        {
            m_Attachments.insert({static_cast<FramebufferAttachment>(index), attachment});
            ++index;
        }

        m_depthAttachment = depth;

        Invalidate();
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

    void OpenGLFrameBuffer::Resize(int w, int h)
    {
        for(const auto & color: m_colorAttachments)
        {
            color->resize(w, h);
        }
        if (m_depthAttachment)
        {
            m_depthAttachment->resize(w, h);
        }
        Invalidate();
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        if (m_RendererID != 0)
        {
            glDeleteFramebuffers(1, &m_RendererID);
        }

        glCreateFramebuffers(1, &m_RendererID);
        Bind();


        /// TODO: ................

        auto index = 0;
        std::vector<GLenum> drawBuffers;
        auto target = GL_TEXTURE_2D;
        for (const auto& texture : m_colorAttachments)
        {
            drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + index);

            if (texture->multiSampleFlag())
            {
                target = GL_TEXTURE_2D_MULTISAMPLE;
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers.back(), target, texture->RenderID(), 0);
            index++;
        }

        GLenum attachment;
        switch (const auto format = m_depthAttachment ? m_depthAttachment->InternalFormat() : GL_DEPTH24_STENCIL8;
        format)
        {
        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
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

        if (m_depthAttachment)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, m_depthAttachment->RenderID(), 0);
        }

        glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
            // TODO: make check as func
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFrameBuffer::RenderID()
    {
        return m_RendererID;
    }

    std::shared_ptr<Texture> OpenGLFrameBuffer::GetAttachment(const FramebufferAttachment& index)
    {
        if (index == FramebufferAttachment::Depth)
        {
            return m_depthAttachment;
        }

        OBASE_ASSERT(m_Attachments.count(index), "No Attchement")
        return m_Attachments.at(index);
    }
}