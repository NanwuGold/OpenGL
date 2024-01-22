
#include "Texture.h"
#include "OpenGL/OpenGLTexture.h"

FXAA::Ref<Texture> Texture::Create(int w, int h, GLenum format)
{
    return FXAA::CreateRef<OpenGLTexture>(w,h,format);
}
