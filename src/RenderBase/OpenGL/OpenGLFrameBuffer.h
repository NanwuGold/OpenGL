#ifndef OPENGL_FXAA_OPENGLFRAMEBUFFER_H
#define OPENGL_FXAA_OPENGLFRAMEBUFFER_H

#include "RenderBase/Render/FrameBuffer.h"

class OpenGLFrameBuffer :public FrameBuffer
{
public:
    OpenGLFrameBuffer();

    void Create(std::vector<OBase::Ref<Texture>> colorAttachments, OBase::Ref<Texture> depth) override;

    void Bind() override;

    void Resize(int w, int h) override;

    void UnBind() override;

private:
    void Invalidate();

private:
    unsigned int m_RendererID;
    std::vector<OBase::Ref<Texture>> m_colorAttachments;
    OBase::Ref<Texture> m_depthAttachment;

};

#endif //OPENGL_FXAA_OPENGLFRAMEBUFFER_H
