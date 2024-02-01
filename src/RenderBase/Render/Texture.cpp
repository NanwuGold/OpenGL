
#include "Texture.h"
#include "RenderBase/OpenGL/OpenGLTexture.h"

OBase::Ref<Texture> Texture::Create(int w, int h, GLenum format)
{
    return OBase::CreateRef<OpenGLTexture>(w, h, format);
}
