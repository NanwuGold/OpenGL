#ifndef OPENGL_RENDERBASE_TEXTURE_H
#define OPENGL_RENDERBASE_TEXTURE_H

#include "RenderBase/pointer_ptr.hpp"
#include <glad/glad.h>

class Texture
{
public:

    Texture() = default;

    Texture(Texture&&) = delete;
    Texture(Texture&) = delete;

    Texture& operator=(Texture&&) = delete;
    Texture& operator=(Texture&) = delete;

    virtual ~Texture() = default;

    virtual void Bind() = 0;
    virtual void resize(int w,int h) = 0;
    virtual void Create() = 0;
    virtual void Invalidate() = 0;

    virtual GLenum format() = 0;
    virtual unsigned int RenderID() = 0;

    virtual bool multiSampleFlag() = 0;

    static OBase::Ref<Texture> Create(int w, int h, GLenum format, bool multiSampleFlag = false);
};


#endif //OPENGL_RENDERBASE_TEXTURE_H
