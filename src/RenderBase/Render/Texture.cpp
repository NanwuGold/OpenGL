
#include "Texture.h"
#include "RenderBase/OpenGL/OpenGLTexture.h"

OBase::Ref<Texture> Texture::Create(int w, int h, GLenum format, MultiSample sample)
{
    if (sample == MultiSample::None)
    {
        return OBase::CreateRef<OpenGLTexture>(w, h, format);
    }

    return OBase::CreateRef<OpenGLMultiSampleTexture>(w, h, format, sample);
}
