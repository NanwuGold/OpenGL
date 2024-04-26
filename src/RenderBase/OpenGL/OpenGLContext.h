#ifndef  OPENGL_RENDERBASE_OPENGLCONTEXT_H
#define  OPENGL_RENDERBASE_OPENGLCONTEXT_H

#include <memory>
#include "RenderBase/Render/Context.h"


struct GLFWwindow;

namespace OBase
{
    class OpenGLContext : public Context
    {
    public:
        explicit OpenGLContext(std::shared_ptr<GLFWwindow> window);
        ~OpenGLContext() override;

        void Init() override;

        void SwapBuffers() override;

    private:
        std::shared_ptr<GLFWwindow> m_Window;

    };
}




#endif // ! OPENGL_RENDERBASE_OPENGLCONTEXT_H
