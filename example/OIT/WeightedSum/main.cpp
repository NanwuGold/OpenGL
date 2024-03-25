#include <iostream>

/**
 * @brief Meshkin's Method do OIT
 */

#include <Platform/WindowBase.h>

#include <RenderBase/OpenGL/OpenGLContext.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/OpenGL/OpenGLShader.h>
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/Render/Buffer.h"

#include <glm/gtc/type_ptr.hpp>


int main()
{
    glfwInit();

    OBase::WindowPro windowPro(800,600,"Weighted Sum");

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(windowPro.width(),windowPro.height(),windowPro.title().c_str(),nullptr,nullptr),[](GLFWwindow* w){});
    const auto context = OBase::CreateRef<OBase::OpenGLContext>(window);
    context->Init();

    const auto triangleShader = std::make_shared<OpenGLShader>("./Shaders/triangle.vert", "./Shaders/triangle.frag");
    const auto cubeVertexArray = OBase::VertexArray::Create();
    {
        float vertices[3 * 7] =
                {
                        -.5f, -.5f, .5f, 0.0f, 0.2f, 0.8f, 1.0f,
                        .5f, -.5f, .5f, 0.2f, 0.4f, 0.8f, 1.0f,
                        .0f, +.5f, .5f, 0.8f, 0.8f, 0.2f, 1.0f
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

    float depthValue = 0.0;
    while(!glfwWindowShouldClose(window.get()))
    {
        glfwPollEvents();
        glBindFramebuffer(GL_FRAMEBUFFER,0);

        glClearBufferfv(GL_COLOR,0,glm::value_ptr(glm::vec3(0.1,0.3,0.5)));
        glClearDepth(depthValue);
        glClear(GL_DEPTH_BUFFER_BIT);

        triangleShader->Bind();
        cubeVertexArray->Bind();
        glDrawElements(GL_TRIANGLES,cubeVertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);

        context->SwapBuffers();
    }

    glfwTerminate();


    return 0;
}