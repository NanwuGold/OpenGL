#include "UniformBufferLayer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <RenderBase/OpenGL/OpenGLShader.h>
#include <RenderBase/Render/Buffer.h>
#include <RenderBase/Render/VertexArray.h>
#include <RenderBase/Core/Application.h>
#include <RenderBase/Camera/CameraFunc.h>

#include <imgui.h>

#include <array>

namespace OBase
{
    void UniformBufferLayer::OnEvent(Event &event)
    {
        Layer::OnEvent(event);
    }

    UniformBufferLayer::UniformBufferLayer(const std::string &name)
            : Layer(name)
    {
    }

    UniformBufferLayer::~UniformBufferLayer()
    {
    }

    void UniformBufferLayer::OnAttach()
    {
        Init();
    }

    void UniformBufferLayer::OnDetach()
    {
        Destory();
    }

    void UniformBufferLayer::OnUpdate(Timestep ts)
    {
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_opaqueBackgroundColor));
        m_TriangleShader->Bind();

        glm::mat4 model(1.0);
        CameraFunc cameraFunc(m_box, glm::vec3 (glm::vec3(0.0) - glm::vec3(0.0,0.0,3.0)));
        auto view = glm::lookAt(cameraFunc.getPosition(), cameraFunc.getTarget(), glm::vec3(0,1.0,0.0));
        auto min = m_box.min();
        auto max = m_box.max();
        auto projection = glm::ortho(min.x,max.x,min.y,max.y,0.01,10.0);

        m_TriangleShader->setMat4("model", model);
        m_TriangleShader->setMat4("view", view);
        m_TriangleShader->setMat4("project", projection);
        m_CaseVertexArray->Bind();

        glDrawElements(GL_TRIANGLES, m_CaseVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    }

    void UniformBufferLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings panel");
        ImGui::ColorEdit4("backgroundColor", glm::value_ptr(m_opaqueBackgroundColor), ImGuiColorEditFlags_NoAlpha);
        ImGui::End();
    }

    void UniformBufferLayer::Init()
    {
        m_TriangleShader = CreateRef<OpenGLShader>("./Shaders/case_triangle.vert", "./Shaders/case_triangle.frag");
        m_box = BoundingBox(glm::dvec3(-2),glm::dvec3(2));

        m_CaseVertexArray = VertexArray::Create();
        {
            float vertices[3 * 7] =
                    {
                            -.5f, -.5f, .0f, 0.0f, 0.2f, 0.8f, 1.0f,
                            .5f, -.5f, .0f, 0.2f, 0.4f, 0.8f, 1.0f,
                            .0f, +.5f, .0f, 0.8f, 0.8f, 0.2f, 1.0f
                    };

            const auto vertexBuffer = (VertexBuffer::Create(vertices, sizeof(vertices)));

            vertexBuffer->Bind();
            {
                const BufferLayout layout = {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float4, "a_Color"}
                };
                vertexBuffer->SetLayout(layout);
            }

            m_CaseVertexArray->AddVertexBuffer(vertexBuffer);

            uint32_t indices[3] = {0, 1, 2};
            const auto indexBuffer = (IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
            indexBuffer->Bind();

            m_CaseVertexArray->SetIndexBuffer(indexBuffer);
        }
        // const auto &window = OBase::Application::Get().GetWindow();

    }

    void UniformBufferLayer::Destory()
    {
        m_CaseVertexArray.reset();
        m_TriangleShader.reset();
    }
} // OBase