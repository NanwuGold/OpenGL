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
    auto quad_shader = std::make_shared<Shader>("./Shaders/FXAA_VS_quad.glsl","./Shaders/FXAA_FS_quad.glsl");

    auto m_CubeVertexArray = FXAA::VertexArray::Create();
    {
        float vertices[3 * 3] =
                {
//                        -.5f, -.5f, .0f,  //0.8f, 0.2f, 0.8f, 1.0f,
//                        .5f, -.5f, .0f, //0.2f, 0.4f, 0.8f, 1.0f,
//                        .0f, +.5f, .0f, // 0.8f, 0.8f, 0.2f, 1.0f

                0.5,0.0,0.0,
                -0.5,0.5,0.0,
                0.,-0.5,0.0
                };

        auto m_VertexBuffer = (FXAA::VertexBuffer::Create(vertices, sizeof(vertices)));

        m_VertexBuffer->Bind();
        {
            FXAA::BufferLayout layout = {
                    {FXAA::ShaderDataType::Float3, "a_Position"},
                    // {FXAA::ShaderDataType::Float4, "a_Color"}
            };
            m_VertexBuffer->SetLayout(layout);
        }

        m_CubeVertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        auto m_IndexBuffer = (FXAA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_IndexBuffer->Bind();

        m_CubeVertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    auto m_QuadVertexArray = FXAA::VertexArray::Create();
    {
        float vertices[4 * 5] =
        {
            -1.0,-1.0,0.0,0.0,0.0,  ///< 左下
            1.0,-1.0,0.0,1.0,0.0,   ///< 右下
            1.0,1.0,0.0,1.0,1.0,  ///< 右上
            -1.0,1.0,0.0,0.0,1.0  ///< 左上
        };

        auto m_VertexBuffer = (FXAA::VertexBuffer::Create(vertices, sizeof(vertices)));

        m_VertexBuffer->Bind();
        {
            FXAA::BufferLayout layout = {
                    {FXAA::ShaderDataType::Float3, "a_Position"},
                    {FXAA::ShaderDataType::Float2, "a_TexCoord"}
            };
            m_VertexBuffer->SetLayout(layout);
        }

        m_QuadVertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[6] = {0, 1, 2,0,2,3};
        auto m_IndexBuffer = (FXAA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_IndexBuffer->Bind();

        m_QuadVertexArray->SetIndexBuffer(m_IndexBuffer);
    }


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
