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

#include <glm/gtc/type_ptr.hpp>

struct WindowData
{
    std::string title;
    int w = 800;
    int h = 600;
};

namespace MSAA
{
    WindowData g_windowPro{"MSAA",1200,1000};

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

void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main()
{
    glfwInit();

    std::shared_ptr<GLFWwindow> window(glfwCreateWindow(MSAA::Window::width(),MSAA::Window::height(),MSAA::Window::title().c_str(),nullptr,nullptr),[](GLFWwindow *window){
        window = nullptr;
    });
    const auto context = std::make_shared<OBase::OpenGLContext>(window);
    context->Init();

    const auto triangleShader = std::make_shared<OpenGLShader>("./Shaders/triangle.vert", "./Shaders/triangle.frag");

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
        cubeVertexArray->SetIndexBuffer(indexBuffer);
        cubeVertexArray->SetIndexBuffer(indexBuffer);
        cubeVertexArray->SetIndexBuffer(indexBuffer);
        cubeVertexArray->SetIndexBuffer(indexBuffer);
    }

    glfwSetErrorCallback(errorCallback);

    auto sample = MultiSample::X16;
    auto multiColorTexture = std::make_shared<OpenGLMultiSampleTexture>(MSAA::Window::width(), MSAA::Window::height(), GL_RGBA8,sample);
    multiColorTexture->Create();

    const auto multiDepthTexture = std::make_shared<OpenGLMultiSampleTexture>(MSAA::Window::width(), MSAA::Window::height(), GL_DEPTH24_STENCIL8,sample);
    multiDepthTexture->Create();

    const auto multiFbo = FrameBuffer::Create();
    multiFbo->Create({ multiColorTexture }, multiDepthTexture);

    multiFbo->Bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    auto color = Texture::Create(MSAA::Window::width(), MSAA::Window::height(), GL_RGBA32F); color->Create();
    const auto depth = Texture::Create(MSAA::Window::width(), MSAA::Window::height(), GL_DEPTH24_STENCIL8); depth->Create();

    const auto fbo = FrameBuffer::Create();
    fbo->Create({ color }, depth);

    int sampleint = 16;
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a multisampled color attachment texture
    unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sampleint, GL_RGB, MSAA::Window::width(), MSAA::Window::height(), GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleint, GL_DEPTH24_STENCIL8, MSAA::Window::width(), MSAA::Window::height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    unsigned int intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
    // create a color attachment texture
    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MSAA::Window::width(), MSAA::Window::height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

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
    const auto quadShader = std::make_shared<OpenGLShader>("./Shaders/triangle_withTexture.vert", "./Shaders/triangle_withTexture.frag");

    while(!glfwWindowShouldClose(window.get()))
    {
        glfwPollEvents();


#if  0
        glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
        glm::vec4 clearColor{1.0,1.0,1.0,1.0};
        glClearBufferfv(GL_COLOR,0,glm::value_ptr(clearColor));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        triangleShader->Bind();
        cubeVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, cubeVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitFramebuffer(0, 0, MSAA::Window::width(), MSAA::Window::height(), 0, 0, MSAA::Window::width(), MSAA::Window::height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);


        /// show it 
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(clearColor));
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        quadShader->Bind();
        quadVertexArray->Bind();

        quadShader->setInt("texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glDrawElements(GL_TRIANGLES, quadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
#else
        multiFbo->Bind();
        glm::vec4 clearColor{ 1.0,1.0,1.0,1.0 };
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(clearColor));
        glClear(GL_DEPTH_BUFFER_BIT);

        triangleShader->Bind();
        cubeVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, cubeVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, multiFbo->RenderID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->RenderID());
        glBlitFramebuffer(0, 0, MSAA::Window::width(), MSAA::Window::height(), 0, 0, MSAA::Window::width(), MSAA::Window::height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

        /// show it 
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(clearColor));
        glClear(GL_DEPTH_BUFFER_BIT);

        quadShader->Bind();
        quadVertexArray->Bind();

        quadShader->setInt("texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, color->RenderID());
        glDrawElements(GL_TRIANGLES, quadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


#endif 


        context->SwapBuffers();

    }

    glfwTerminate();

    return 0;
}



