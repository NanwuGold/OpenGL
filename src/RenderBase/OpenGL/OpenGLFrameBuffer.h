#ifndef OPENGL_RENDERBASE_OPENGLFRAMEBUFFER_H
#define OPENGL_RENDERBASE_OPENGLFRAMEBUFFER_H

#include <unordered_map>

#include "RenderBase/Render/FrameBuffer.h"

class OpenGLFrameBuffer :public FrameBuffer
{
public:

    OpenGLFrameBuffer();

    ~OpenGLFrameBuffer() override;

    OpenGLFrameBuffer(const OpenGLFrameBuffer&) = delete;
    OpenGLFrameBuffer(const OpenGLFrameBuffer&&) = delete;

    OpenGLFrameBuffer& operator=(const OpenGLFrameBuffer&) = delete;
    OpenGLFrameBuffer& operator=(const OpenGLFrameBuffer&&) = delete;


    void Create(std::vector<OBase::Ref<Texture>> colorAttachments, OBase::Ref<Texture> depth) override;

    void Bind() override;

    void Resize(int w, int h) override;

    void UnBind() override;

    unsigned RenderID() override;

    std::shared_ptr<Texture> GetAttachment(FramebufferAttachment& index) override;


private:
    void Invalidate();

private:
    unsigned int m_RendererID;
    std::vector<OBase::Ref<Texture>> m_colorAttachments;

    std::unordered_map<FramebufferAttachment, OBase::Ref<Texture>> m_Attachments;

    OBase::Ref<Texture> m_depthAttachment;

};

#endif //OPENGL_RENDERBASE_OPENGLFRAMEBUFFER_H
