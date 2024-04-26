#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OBase
{
    Application::Application(const std::string &name)
    {
        m_Window = std::shared_ptr<Window>(Window::Create(WindowProps(name)));
    }

    void Application::run()
    {
        while (m_Running)
        {
            m_Running = !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));

            glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(1.0f)));



            m_Window->OnUpdate();
        }
    }

    Application::~Application()
    {
        m_Running = false;
        glfwTerminate();

        m_Window.reset();

    }
} // OBase