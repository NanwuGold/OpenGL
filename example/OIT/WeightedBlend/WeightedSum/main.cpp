/**
 * @brief Meshkin's Method do OIT
 */

#include <iostream>

#include "Platform/WindowBase.h"

#include "RenderBase/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderBase/pointer_ptr.hpp"
#include "RenderBase/OpenGL/OpenGLShader.h"
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/Render/Buffer.h"
#include "RenderBase/Render/Texture.h"
#include "RenderBase/Render/FrameBuffer.h"

#include <glm/gtc/type_ptr.hpp>

int main()
{
    glfwInit();

    OBase::WindowPro windowPro(800,600,"Weighted Sum");

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(windowPro.width(),windowPro.height(),windowPro.title().c_str(),nullptr,nullptr),[](GLFWwindow* w){});
    const auto context = OBase::CreateRef<OBase::OpenGLContext>(window);
    context->Init();

    const auto triangleShader = std::make_shared<OpenGLShader>("./Shaders/WS_triangle.vert", "./Shaders/WS_triangle.frag");
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

    const auto quadVertexArray = OBase::VertexArray::Create();
    {
        float vertices[4 * 5] =
                {
                    -1.0,-1.0,0.0,0.0,0.0,  ///< 左下
                    1.0,-1.0,0.0,1.0,0.0,   ///< 右下
                    1.0,1.0,0.0,1.0,1.0,  ///< 右上
                    -1.0,1.0,0.0,0.0,1.0  ///< 左上
                };

        const auto vertexBuffer = (OBase::VertexBuffer::Create(vertices, sizeof(vertices)));

        vertexBuffer->Bind();
        {
            const OBase::BufferLayout layout = {
                    {OBase::ShaderDataType::Float3, "a_Position"},
                    {OBase::ShaderDataType::Float2, "a_TexCoord"}
            };
            vertexBuffer->SetLayout(layout);
        }

        quadVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[6] = { 0, 1, 2,0,2,3 };
        const auto indexBuffer = (OBase::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        indexBuffer->Bind();

        quadVertexArray->SetIndexBuffer(indexBuffer);
    }
    const auto quadShader = std::make_shared<OpenGLShader>("./Shaders/WS_blend.vert", "./Shaders/WS_blend.frag");


    auto color = Texture::Create(windowPro.width(),windowPro.height(),GL_RGBA32F,MultiSample::None);
    color->Create();

    auto depth = Texture::Create(windowPro.width(),windowPro.height(),GL_DEPTH32F_STENCIL8,MultiSample::None);
    depth->Create();

    auto accumeBuffer = FrameBuffer::Create();
    accumeBuffer->Create({color},depth);


    auto backgroundFB = FrameBuffer::Create();
    {
        auto color = Texture::Create(windowPro.width(),windowPro.height(),GL_RGBA32F,MultiSample::None);
        color->Create();

        auto depth = Texture::Create(windowPro.width(),windowPro.height(),GL_DEPTH32F_STENCIL8,MultiSample::None);
        depth->Create();

        backgroundFB->Create({color},depth);
    }

    float depthValue = 0.0;
    while(!glfwWindowShouldClose(window.get()))
    {
        glfwPollEvents();

        /// Copy background Texture
        backgroundFB->Bind();
        glClearBufferfv(GL_COLOR,0,glm::value_ptr(glm::vec4(0.3,0.4,0.5,1.0)));

        accumeBuffer->Bind();
        glClearBufferfv(GL_COLOR,0,glm::value_ptr(glm::vec4(0.0,0.0,0.0,0.0)));

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE,GL_ONE);

        triangleShader->Bind();

        // resolution
        triangleShader->setMat4("model",glm::translate(glm::mat4(1.0f),glm::vec3(0,0,-.3)));
        triangleShader->setVec4("i_showColor",glm::vec4(1.0,0.0,0.0,0.4));
        cubeVertexArray->Bind();
        glDrawElements(GL_TRIANGLES,cubeVertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);

        triangleShader->setMat4("model",glm::translate(glm::mat4(1.0f),glm::vec3(0.1,0,-0.5)));
        triangleShader->setVec4("i_showColor",glm::vec4(0.0,1.0,0.0,0.4));
        glDrawElements(GL_TRIANGLES,cubeVertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);

        triangleShader->setMat4("model",glm::translate(glm::mat4(1.0f),glm::vec3(0.2,0,-0.7)));
        triangleShader->setVec4("i_showColor",glm::vec4(0.0,0.0,1.0,0.4));
        glDrawElements(GL_TRIANGLES,cubeVertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT,nullptr);

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClearBufferfv(GL_COLOR,0,glm::value_ptr(glm::vec3(0.0,0.0,0.0)));
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);

        quadShader->Bind();
        quadVertexArray->Bind();

        quadShader->setInt("texture1",0);
        glActiveTexture(GL_TEXTURE0);
        backgroundFB->GetAttachment(FramebufferAttachment::Color0)->Bind();

        quadShader->setInt("texture2",1);
        glActiveTexture(GL_TEXTURE1);
        accumeBuffer->GetAttachment(FramebufferAttachment::Color0)->Bind();

        glDrawElements(GL_TRIANGLES,quadVertexArray->GetIndexBuffer()->GetCount(),GL_UNSIGNED_INT, nullptr);

        context->SwapBuffers();
    }

    glfwTerminate();


    return 0;
}