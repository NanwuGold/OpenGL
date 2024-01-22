#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Render/Shader.h>
#include <Render/VertexArray.h>
#include <Render/Buffer.h>
#include <Render/Texture.h>
#include <Render/FrameBuffer.h>
#include "processInput.h"

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

    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Fast approximate anti-aliasing", nullptr, nullptr);

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

    glViewport(0, 0, 800, 600);

    auto cube_shader = std::make_shared<Shader>("./Shaders/FXAA_VS_drawCube.glsl","./Shaders/FXAA_FS_drawCube.glsl");

    auto m_VertexArray = FXAA::VertexArray::Create();

    float vertices[3 * 7] =
    {
            -.5f, -.5f, .0f, 0.8f, 0.2f, 0.8f, 1.0f,
            .5f, -.5f, .0f, 0.2f, 0.4f, 0.8f, 1.0f,
            .0f, +.5f, .0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    auto m_VertexBuffer = (FXAA::VertexBuffer::Create(vertices, sizeof(vertices)));

    m_VertexBuffer->Bind();
    {
        FXAA::BufferLayout layout = {
                {FXAA::ShaderDataType::Float3, "a_Position"},
                {FXAA::ShaderDataType::Float4, "a_Color"}
        };
        m_VertexBuffer->SetLayout(layout);
    }

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    uint32_t indices[3] = {0, 1, 2};
    auto m_IndexBuffer = (FXAA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_IndexBuffer->Bind();

    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    auto color = Texture::Create(800,600,GL_RGBA32F); color->Create();
    auto depth = Texture::Create(800,600,GL_DEPTH24_STENCIL8); depth->Create();

    auto fbo = FrameBuffer::Create();
    fbo->Create({color},depth);

    while(!glfwWindowShouldClose(window))
    {
        FXAA::processor(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
        {
            fbo->Bind();

            glClearColor(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);


            cube_shader->Bind();
            m_VertexArray->Bind();

            const auto count = m_VertexArray->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
        }

        fbo->UnBind();
        {
            glClearColor(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);

            cube_shader->Bind();
            m_VertexArray->Bind();

            const auto count = m_VertexArray->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
        }
    }

    glfwTerminate();
    return 0;
}
