#ifndef OPENGL_FXAA_OPENGLFRAMEBUFFER_H
#define OPENGL_FXAA_OPENGLFRAMEBUFFER_H

#include <Render/FrameBuffer.h>

class OpenGLFrameBuffer :public FrameBuffer
{
public:
    OpenGLFrameBuffer();

    void Create(std::vector<FXAA::Ref<Texture>> colorAttachments, FXAA::Ref<Texture> depth) override;

    void Bind() override;

    void Resize(int w, int h) override;

    void UnBind() override;

private:
    void Invalidate();

private:
    unsigned int m_RendererID;
    std::vector<FXAA::Ref<Texture>> m_colorAttachments;
    FXAA::Ref<Texture> m_depthAttachment;

};

#endif //OPENGL_FXAA_OPENGLFRAMEBUFFER_H
