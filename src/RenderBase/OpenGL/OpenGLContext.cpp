#include "OpenGLContext.h"
#include "RenderBase/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

namespace OBase
{
    OpenGLContext::OpenGLContext(std::shared_ptr<GLFWwindow> window)
    :m_Window(std::move(window))
    {

    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_Window.get());

        const auto status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        OBASE_ASSERT(status,"Failed to initialize Glad!")

    	/// TODO: output OpenGL Infos
        const auto vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        const auto render = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        const auto version = reinterpret_cast<const char *>(glGetString(GL_VERSION));

        OBASE_INFO("OpenGL Vendor: "   + std::string(vendor))
        OBASE_INFO("OpenGL Renderer: " + std::string(render))
        OBASE_INFO("OpenGL Renderer: " + std::string(version))
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_Window.get());
    }

    OpenGLContext::~OpenGLContext()
    {
        m_Window = nullptr;
    }
}
