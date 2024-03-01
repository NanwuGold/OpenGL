
#include "Texture.h"
#include "RenderBase/OpenGL/OpenGLTexture.h"

OBase::Ref<Texture> Texture::Create(int w, int h, GLenum format, bool multiSampleFlag)
{
    if (multiSampleFlag)
    {
        
    }
    return OBase::CreateRef<OpenGLTexture>(w, h, format);

}
