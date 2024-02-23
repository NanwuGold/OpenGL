#include <iostream>

#include "RenderBase/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowData
{
    std::string title;
    int w = 800;
    int h = 600;
};

namespace MSAA
{
    WindowData g_windowPro{"MSAA",800,600};

    namespace Window
    {
        int width()
        {
            return g_windowPro.w;
        }

        int height()
        {
            return g_windowPro.h;
        }

        std::string title()
        {
            return g_windowPro.title;
        }
    }

}

int main()
{
    std::cout<<"Hello world"<<std::endl;

    glfwInit();

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(MSAA::Window::width(),MSAA::Window::height(),MSAA::Window::title().c_str(),nullptr,nullptr),[](GLFWwindow *window){
        window = nullptr;
    });
    auto context = std::make_shared<OBase::OpenGLContext>(window);
    context->Init();

    while(!glfwWindowShouldClose(window.get()))
    {
        context->SwapBuffers();
        glfwPollEvents();

        glClearColor(0.1,0.3,0.4,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    }

    glfwTerminate();

    return 0;
}



