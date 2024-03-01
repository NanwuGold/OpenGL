#include <iostream>

#include "RenderBase/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderBase/OpenGL/OpenGLShader.h"
#include "RenderBase/OpenGL/OpenGLTexture.h"
#include "RenderBase/Render/Buffer.h"
#include "RenderBase/Render/FrameBuffer.h"
#include "RenderBase/Render/Texture.h"
#include "RenderBase/Render/VertexArray.h"

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
    glfwInit();

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(MSAA::Window::width(),MSAA::Window::height(),MSAA::Window::title().c_str(),nullptr,nullptr),[](GLFWwindow *window){
        window = nullptr;
    });
    const auto context = std::make_shared<OBase::OpenGLContext>(window);
    context->Init();

    const auto quadShader = std::make_shared<OpenGLShader>("./Shaders/triangle.vert", "./Shaders/triangle.frag");

    const auto cubeVertexArray = OBase::VertexArray::Create();
    {
        float vertices[3 * 7] =
        {
            -.5f, -.5f, .0f, 0.8f, 0.2f, 0.8f, 1.0f,
            .5f, -.5f, .0f, 0.2f, 0.4f, 0.8f, 1.0f,
            .0f, +.5f, .0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        const auto vertexBuffer = (OBase::VertexBuffer::Create(vertices, sizeof(vertices)));

        vertexBuffer->Bind();
        {
            const OBase::BufferLayout layout = {
                    { OBase::ShaderDataType::Float3, "a_Position"},
                     {OBase::ShaderDataType::Float4, "a_Color"}
            };
            vertexBuffer->SetLayout(layout);
        }

        cubeVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        const auto indexBuffer = (OBase::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        indexBuffer->Bind();

        cubeVertexArray->SetIndexBuffer(indexBuffer);
    }


    auto color = std::make_shared<OpenGLMultiSampleTexture>(MSAA::Window::width(), MSAA::Window::height(), GL_RGBA32F);
    color->Create();

    const auto depth = std::make_shared<OpenGLMultiSampleTexture>(MSAA::Window::width(), MSAA::Window::height(), GL_DEPTH24_STENCIL8);
    depth->Create();

    const auto fbo = FrameBuffer::Create();
    fbo->Create({ color }, depth);


    while(!glfwWindowShouldClose(window.get()))
    {
        context->SwapBuffers();
        glfwPollEvents();


#if 0

        fbo->Bind();

        glClearColor(0.1,0.3,0.4,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quadShader->Bind();
        cubeVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, cubeVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.3, 0.3, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fbo->Bind();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, MSAA::Window::width(), MSAA::Window::height(), 0, 0, MSAA::Window::width(), MSAA::Window::height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif

        glClearColor(0.1, 0.3, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, cubeVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


    }

    glfwTerminate();

    return 0;
}



