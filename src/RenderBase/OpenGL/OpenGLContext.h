#ifndef  OPENGL_RENDERBASE_OPENGLCONTEXT_H
#define  OPENGL_RENDERBASE_OPENGLCONTEXT_H

#include "RenderBase/Render/Context.h"
#include <GLFW/glfw3.h>

namespace OBase
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(GLFWwindow * window);

        void Init() override;

        void SwapBuffers() override;

    private:
        GLFWwindow * m_Window;

    };
}




#endif // ! OPENGL_RENDERBASE_OPENGLCONTEXT_H
