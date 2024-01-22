#ifndef OPENGL_FXAA_FRAMEBUFFER_H
#define OPENGL_FXAA_FRAMEBUFFER_H

#include <vector>
#include "pointer_ptr.hpp"
#include "Texture.h"

class FrameBuffer
{
public:
    virtual void Create(std::vector<FXAA::Ref<Texture>> colorAttachments,FXAA::Ref<Texture> depth) = 0;
    virtual void Bind() = 0;
    virtual void UnBind() = 0;
    virtual void Resize(int w, int h) = 0;
    static FXAA::Ref<FrameBuffer> Create();

};


#endif //OPENGL_FXAA_FRAMEBUFFER_H
