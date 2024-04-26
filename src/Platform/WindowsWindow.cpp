#include "WindowsWindow.h"

#include <RenderBase/OpenGL/OpenGLContext.h>
#include <GLFW/glfw3.h>

#include "RenderBase/Core/Core.h"

#include <iostream>

namespace OBase
{
    namespace 
    {
        bool g_SGlfwInitialized = false;

    }

    Window *Window::Create(const OBase::WindowProps &props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        delete m_Context;
        ShutDown();
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    uint32_t WindowsWindow::GetWidth() const
    {
        return m_Data.m_Width;
    }

    uint32_t WindowsWindow::GetHeight() const
    {
        return m_Data.m_Height;
    }

    void WindowsWindow::SetVSync(bool enable)
    {
        if (enable)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
        m_Data.m_VSync = enable;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.m_VSync;;
    }

    void *WindowsWindow::GetNativeWindow() const
    {
        return m_Window;
    }

    void WindowsWindow::Init(const WindowProps & props)
    {
        m_Data.m_Title = props.m_Title;
        m_Data.m_Width = props.m_W;
        m_Data.m_Height = props.m_H;
        
        if (!g_SGlfwInitialized)
        {
            int success = false;
            {
                success = glfwInit();
            }
            OBASE_ASSERT(success, "glfw Init Faild!")
            g_SGlfwInitialized = true;
        }
        m_Window = glfwCreateWindow(props.width(), props.height(), props.m_Title.c_str(), nullptr, nullptr);

        const auto window = std::shared_ptr<GLFWwindow>(m_Window,[](GLFWwindow * w){});
        m_Context = new OpenGLContext(window);
        m_Context->Init();
    }

    void WindowsWindow::ShutDown()
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }


} // OBase