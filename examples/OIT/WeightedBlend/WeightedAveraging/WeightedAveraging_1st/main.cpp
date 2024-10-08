/**
 * @brief Bavoil和Myer的方法实现Weighted Blend
 * @note 该公式也有个缺点，后续版本也一样，如果不透明的实体使用该公式绘制，C0项目为0， 但公式前半部分 变为 Ci 求和项 除于 ai 求和项，  平均了颜色，所以明明不透明的实体也变得透明了。
 */

#include "RenderBase/Core/Window.h"

#include "RenderBase/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderBase/pointer_ptr.hpp"

#include "RenderBase/OpenGL/OpenGLShader.h"
#include "RenderBase/Render/Buffer.h"
#include "RenderBase/Render/FrameBuffer.h"
#include "RenderBase/Render/VertexArray.h"

int main()
{
    OBase::WindowProps windowPro(800, 600, " Weighted Averaging 1st");
    glfwInit();

    std::shared_ptr<GLFWwindow> window(
        glfwCreateWindow(windowPro.width(), windowPro.height(), windowPro.title().c_str(), nullptr, nullptr), [
        ](GLFWwindow*)
        {
        });
    const auto context = OBase::CreateRef<OBase::OpenGLContext>(window);

    context->Init();
    const auto triangleShader = std::make_shared<OpenGLShader>("./Shaders/WA1_triangle.vert", "./Shaders/WA1_triangle.frag");
    const auto triangleVertexArray = OBase::VertexArray::Create();
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

        triangleVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        const auto indexBuffer = (OBase::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        indexBuffer->Bind();

        triangleVertexArray->SetIndexBuffer(indexBuffer);
    }

    const auto screenVertexArray = OBase::VertexArray::Create();
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

        screenVertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[6] = { 0, 1, 2,0,2,3 };
        const auto indexBuffer = (OBase::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        indexBuffer->Bind();

        screenVertexArray->SetIndexBuffer(indexBuffer);
    }

    const auto quadShader = std::make_shared<OpenGLShader>("./Shaders/WA1_quad.vert", "./Shaders/WA1_quad.frag");

    const auto accumFB = OBase::FrameBuffer::Create();
    {
        auto color1 = Texture::Create(windowPro.width(), windowPro.height(), GL_RGBA32F, MultiSample::None);
        color1->Create();

        auto color2 = Texture::Create(windowPro.width(), windowPro.height(), GL_RG16F, MultiSample::None);
        color2->Create();

        const auto depth = Texture::Create(windowPro.width(), windowPro.height(), GL_DEPTH32F_STENCIL8, MultiSample::None);
        depth->Create();

        accumFB->Create({ color1,color2}, depth);
    }

    const auto backgroundFB = OBase::FrameBuffer::Create();
    {
        auto color1 = Texture::Create(windowPro.width(), windowPro.height(), GL_RGBA32F, MultiSample::None);
        color1->Create();

        const auto depth = Texture::Create(windowPro.width(), windowPro.height(), GL_DEPTH32F_STENCIL8, MultiSample::None);
        depth->Create();

        backgroundFB->Create({ color1}, depth);
    }

    quadShader->Bind();
    quadShader->setInt("texture1", 0);
    quadShader->setInt("texture2", 1);
    quadShader->setInt("texture_PassCount", 2);

    while (!glfwWindowShouldClose(window.get()))
    {
        glfwPollEvents();

        backgroundFB->Bind();
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.3, 0.4, 0.5, 1.0)));
        // 渲染不透明物体

        accumFB->Bind();
        //读取实体渲染的颜色缓冲与深度缓冲 之后渲染半透明物体
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0, 0.0, 0.0, 0.0)));
        glClearBufferfv(GL_COLOR, 1, glm::value_ptr(glm::vec4(0.0, 0.0, 0.0, 0.0)));
        glClear(GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        triangleVertexArray->Bind();
        triangleShader->Bind();

        triangleShader->setMat4("model", glm::mat4(1.0f));
        triangleShader->setVec4("i_showColor", glm::vec4(1.0f,0.0,0.0,0.6));
        glDrawElements(GL_TRIANGLES, triangleVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        triangleShader->setMat4("model", glm::translate(glm::mat4(1.0f),glm::vec3(0.1,0.0,0.3)));
        triangleShader->setVec4("i_showColor", glm::vec4(0.0f, 1.0, 0.0, 0.6));
        glDrawElements(GL_TRIANGLES, triangleVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        triangleShader->setMat4("model", glm::translate(glm::mat4(1.0f),glm::vec3(0.2,0.0,0.1)));
        triangleShader->setVec4("i_showColor", glm::vec4(0.0f, 0.0, 1.0, 0.6));
        glDrawElements(GL_TRIANGLES, triangleVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, -1, glm::value_ptr(glm::vec4(0.3, 0.4, 0.5, 1.0)));

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
//        glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);

        screenVertexArray->Bind();
        quadShader->Bind();

        glActiveTexture(GL_TEXTURE0);
        backgroundFB->GetAttachment(FramebufferAttachment::Color0)->Bind();

        glActiveTexture(GL_TEXTURE1);
        accumFB->GetAttachment(FramebufferAttachment::Color0)->Bind();

        glActiveTexture(GL_TEXTURE2);
        accumFB->GetAttachment(FramebufferAttachment::Color1)->Bind();

        glDrawElements(GL_TRIANGLES, screenVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        context->SwapBuffers();
    }

    glfwTerminate();

    window.reset();
    return 0;
}