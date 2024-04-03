#include <iostream>
#include <Platform/WindowBase.h>

#include <RenderBase/OpenGL/OpenGLContext.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <RenderBase/pointer_ptr.hpp>

int main()
{
    OBase::WindowPro windowPro(800,600," Weighted Averaging 1st");
    glfwInit();

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(windowPro.width(),windowPro.height(),windowPro.title().c_str(),nullptr,nullptr),[](GLFWwindow *){});
    const auto context = OBase::CreateRef<OBase::OpenGLContext>(window);

    context->Init();


    while(!glfwWindowShouldClose(window.get()))
    {
        glfwPollEvents();

        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        context->SwapBuffers();
    }

    glfwTerminate();

    window.reset();

    std::cout<<__FUNCTION__<<std::endl;
    return 0;
}
