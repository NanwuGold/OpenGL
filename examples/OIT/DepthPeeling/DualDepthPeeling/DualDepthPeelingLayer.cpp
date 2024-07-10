#include "DualDepthPeelingLayer.h"
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/Core/Application.h"
#include <RenderBase/Render/Buffer.h>
#include <RenderBase/Render/FrameBuffer.h>
#include <RenderBase/Geometry/BoundingBox.h>
#include <RenderBase/Camera/CameraFunc.h>

namespace OBase
{
    DualDepthPeelingLayer::DualDepthPeelingLayer(const std::string &name)
            : OBase::Layer(name)
    {

    }

    DualDepthPeelingLayer::~DualDepthPeelingLayer()
    {

    }

    void DualDepthPeelingLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("BackGround Color",glm::value_ptr(m_BackgroundColor),ImGuiColorEditFlags_NoAlpha);
        ImGui::End();
    }

    void DualDepthPeelingLayer::OnUpdate(Timestep ts)
    {
        Layer::OnUpdate(ts);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_BackgroundColor));

        m_TriangleShader->Bind();

        auto mat = glm::rotate(glm::mat4(1.0),glm::radians(m_rotateY),glm::vec3(0,1.0,0));
        CameraFunc cameraFunc(m_box, glm::vec3 (mat * glm::vec4(0,0,1.0,1.0)));

        auto view = glm::lookAt(cameraFunc.getPosition(), cameraFunc.getTarget(), glm::vec3(0,1.0,0.0));

        auto min = m_box.min();
        auto max = m_box.max();

        auto projection = glm::ortho(min.x,max.x,min.y,max.y,0.01,10.0);

        glm::mat4 model(1.0);

        m_TriangleShader->setMat4("model",model);
        m_TriangleShader->setMat4("view",view);
        m_TriangleShader->setMat4("projection",projection);

        m_DataVertexArray->Bind();

        glDrawElements(GL_TRIANGLES, m_DataVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void DualDepthPeelingLayer::OnEvent(Event &event)
    {
        Layer::OnEvent(event);
    }

    void DualDepthPeelingLayer::OnAttach()
    {
        m_DataVertexArray = VertexArray::Create();
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

            m_DataVertexArray->AddVertexBuffer(vertexBuffer);

            uint32_t indices[3] = {0, 1, 2};
            const auto indexBuffer = (IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
            indexBuffer->Bind();

            m_DataVertexArray->SetIndexBuffer(indexBuffer);
        }

        m_TriangleShader = CreateRef<OpenGLShader>("./Shaders/dual_triangle.vert","./Shaders/dual_triangle.frag");

        const auto &window = OBase::Application::Get().GetWindow();
        int windowWidget = static_cast<int>(window.GetWidth());
        int windowHeight = static_cast<int>(window.GetHeight());

        m_TriangleShader->Bind();
        m_TriangleShader->setVec2("screenSize", glm::vec2(windowWidget,windowHeight));

        m_box = BoundingBox(glm::dvec3(-2),glm::dvec3(2));
    }

    void DualDepthPeelingLayer::OnDetach()
    {
        //destory resources
    }
}