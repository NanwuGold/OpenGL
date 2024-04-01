#ifndef OPENGL_RENDERBASE_FRAMEBUFFER_H
#define OPENGL_RENDERBASE_FRAMEBUFFER_H

#include <vector>
#include "RenderBase/pointer_ptr.hpp"
#include "Texture.h"

enum class FramebufferAttachment: uint32_t
{
    Color0 = GL_COLOR_ATTACHMENT0,
    Color1 = GL_COLOR_ATTACHMENT1,
    Color2 = GL_COLOR_ATTACHMENT2,
    Color3 = GL_COLOR_ATTACHMENT3,
    Color4 = GL_COLOR_ATTACHMENT4,
    Color5 = GL_COLOR_ATTACHMENT5,
    Color6 = GL_COLOR_ATTACHMENT6,
    Color7 = GL_COLOR_ATTACHMENT7,
    Color8 = GL_COLOR_ATTACHMENT8,
    Depth
};


class FrameBuffer
{
public:
    FrameBuffer() = default;
    virtual ~FrameBuffer() = default;

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(const FrameBuffer&&) = delete;

    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&&) = delete;

    virtual void Create(std::vector<OBase::Ref<Texture>> colorAttachments, OBase::Ref<Texture> depth) = 0;
    virtual void Bind() = 0;
    virtual void UnBind() = 0;
    virtual void Resize(int w, int h) = 0;

    virtual unsigned int RenderID() = 0;

    virtual std::shared_ptr<Texture> GetAttachment(const FramebufferAttachment &index) = 0;

    static OBase::Ref<FrameBuffer> Create();

};


#endif //OPENGL_RENDERBASE_FRAMEBUFFER_H
