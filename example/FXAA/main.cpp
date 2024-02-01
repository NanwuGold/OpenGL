#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderBase/OpenGL/OpenGLShader.h"
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/Render/Buffer.h"
#include "RenderBase/Render/Texture.h"
#include "RenderBase/Render/FrameBuffer.h"

#include <iostream>
#include "processInput.h"

struct WindowData
{
    std::string title;
    int w = 800;
    int h = 600;
};

namespace FXAA
{
    WindowData g_windowPro{"Fast approximate anti-aliasing",800,600};

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);   ///< 关闭resize

    GLFWwindow* window = glfwCreateWindow(FXAA::Window::width(), FXAA::Window::height(), FXAA::Window::title().c_str(), nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /// resize event
    glfwSetFramebufferSizeCallback(window,[](GLFWwindow * window,int width,int height)
    {
        ///TODO: maybe
    });

    glfwSetErrorCallback(errorCallback);

    glViewport(0, 0, FXAA::Window::width(), FXAA::Window::height());

    auto cube_shader = std::make_shared<OpenGLShader>("./Shaders/FXAA_VS_drawCube.glsl", "./Shaders/FXAA_FS_drawCube.glsl");
    auto quad_shader = std::make_shared<OpenGLShader>("./Shaders/FXAA_VS_quad.glsl", "./Shaders/FXAA_FS_quad.glsl");

    auto m_CubeVertexArray = OBase::VertexArray::Create();
    {
        float vertices[3 * 7] =
        {
            -.5f, -.5f, .0f, 0.8f, 0.2f, 0.8f, 1.0f,
            .5f, -.5f, .0f, 0.2f, 0.4f, 0.8f, 1.0f,
            .0f, +.5f, .0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        auto m_VertexBuffer = (OBase::VertexBuffer::Create(vertices, sizeof(vertices)));

        m_VertexBuffer->Bind();
        {
            OBase::BufferLayout layout = {
                    { OBase::ShaderDataType::Float3, "a_Position"},
                     {OBase::ShaderDataType::Float4, "a_Color"}
            };
            m_VertexBuffer->SetLayout(layout);
        }

        m_CubeVertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        auto m_IndexBuffer = (OBase::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_IndexBuffer->Bind();

        m_CubeVertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    auto m_QuadVertexArray = OBase::VertexArray::Create();
    {
        float vertices[4 * 5] =
        {
            -1.0,-1.0,0.0,0.0,0.0,  ///< 左下
            1.0,-1.0,0.0,1.0,0.0,   ///< 右下
            1.0,1.0,0.0,1.0,1.0,  ///< 右上
            -1.0,1.0,0.0,0.0,1.0  ///< 左上
        };

        auto m_VertexBuffer = (OBase::VertexBuffer::Create(vertices, sizeof(vertices)));

        m_VertexBuffer->Bind();
        {
            OBase::BufferLayout layout = {
                    {OBase::ShaderDataType::Float3, "a_Position"},
                    {OBase::ShaderDataType::Float2, "a_TexCoord"}
            };
            m_VertexBuffer->SetLayout(layout);
        }

        m_QuadVertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[6] = {0, 1, 2,0,2,3};
        auto m_IndexBuffer = (OBase::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_IndexBuffer->Bind();

        m_QuadVertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    auto color = Texture::Create(FXAA::Window::width(), FXAA::Window::height(),GL_RGBA32F); color->Create();
    auto depth = Texture::Create(FXAA::Window::width(), FXAA::Window::height(),GL_DEPTH24_STENCIL8); depth->Create();

    auto fbo = FrameBuffer::Create();
    fbo->Create({color},depth);

    while(!glfwWindowShouldClose(window))
    {
        OBase::processor(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
        {
            fbo->Bind();

            glClearColor(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);


            cube_shader->Bind();
            m_CubeVertexArray->Bind();

            const auto count = m_CubeVertexArray->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
        }

        fbo->UnBind();
        {
            glClearColor(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            quad_shader->Bind();
            quad_shader->setInt("lightTexture",0);
            quad_shader->setVec2("Size",glm::vec2(800,600));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,color->RenderID());
            m_QuadVertexArray->Bind();

            {
                const auto count = m_QuadVertexArray->GetIndexBuffer()->GetCount();
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count),GL_UNSIGNED_INT,nullptr);
            }
        }
    }

    glfwTerminate();
    return 0;
}
