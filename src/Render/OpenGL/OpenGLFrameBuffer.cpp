#include "OpenGLFrameBuffer.h"

#include <iostream>

OpenGLFrameBuffer::OpenGLFrameBuffer()
    :m_RendererID(0)
{
}

void OpenGLFrameBuffer::Create(std::vector<FXAA::Ref<Texture>> colorAttachments, FXAA::Ref<Texture> depth)
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
    if(!m_RendererID)
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }

    glGenFramebuffers(1, &m_RendererID);

    Bind();

    auto index = 0;
    std::vector<GLenum> drawBuffers;
    for(const auto& texture: m_colorAttachments)
    {
        unsigned int textureI;
        glGenTextures(1,&textureI);

        glBindTexture(GL_TEXTURE_2D,textureI);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + index);
        glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers.back(), GL_TEXTURE_2D, textureI, 0);
        index++;

//        drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + index);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffers.back(), GL_TEXTURE_2D, texture->RenderID(), 0);
//        index++;
    }

    unsigned int textureI;
    glGenTextures(1,&textureI);
    glBindTexture(GL_TEXTURE_2D,textureI);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, 800, 600);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureI, 0);

    // glDrawBuffers(drawBuffers.size(),drawBuffers.data());
}

void OpenGLFrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
