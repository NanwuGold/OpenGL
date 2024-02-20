#ifndef OPENGL_RENDERBASE_FRAMEBUFFER_H
#define OPENGL_RENDERBASE_FRAMEBUFFER_H

#include <vector>
#include "RenderBase/pointer_ptr.hpp"
#include "Texture.h"

class FrameBuffer
{
public:
    virtual void Create(std::vector<OBase::Ref<Texture>> colorAttachments, OBase::Ref<Texture> depth) = 0;
    virtual void Bind() = 0;
    virtual void UnBind() = 0;
    virtual void Resize(int w, int h) = 0;
    static OBase::Ref<FrameBuffer> Create();

};


#endif //OPENGL_RENDERBASE_FRAMEBUFFER_H
