#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Render/Shader.h>
#include <Render/VertexArray.h>
#include <Render/Buffer.h>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glViewport(0, 0, 800, 600);

    auto cube_shader = std::make_shared<Shader>("./Shaders/FXAA_VS_drawCube.glsl","./Shaders/FXAA_FS_drawCube.glsl");

    auto m_VertexArray = FXAA::VertexArray::Create();

    float vertices[3 * 3] = {
            -.5f, -.5f, .0f,
             .5f, -.5f, .0f,
            .0f, +.5f, .0f
    };

    auto m_VertexBuffer = (FXAA::VertexBuffer::Create(vertices, sizeof(vertices)));

    m_VertexBuffer->Bind();
    {
        FXAA::BufferLayout layout = {
                {FXAA::ShaderDataType::Float3, "a_Position"}
        };
        m_VertexBuffer->SetLayout(layout);
    }

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    uint32_t indices[3] = {0, 1, 2};
    auto m_IndexBuffer = (FXAA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_IndexBuffer->Bind();

    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();

        glClearColor(1.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        cube_shader->use();
        m_VertexArray->Bind();

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    glfwTerminate();
    return 0;
}
