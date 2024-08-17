#include "LinkedListLayer.h"

#include <glad/glad.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <RenderBase/Camera/CameraFunc.h>
#include <RenderBase/Core/Application.h>
#include <RenderBase/Core/Core.h>
#include <RenderBase/OpenGL/OpenGLShader.h>
#include <RenderBase/Render/Buffer.h>
#include <RenderBase/Render/UniformBuffer.h>
#include <RenderBase/OpenGL/OpenGLShaderStorageBuffer.h>
#include <RenderBase/Render/VertexArray.h>
#include <RenderBase/Event/ApplicationEvent.h>

#include <iostream>


constexpr uint8_t MaxFragments = 10;

namespace OBase
{
    void LinkedListLayer::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](auto && event)
                                               {
                                                   OnResizeEvent(std::forward<decltype(event)>(event));
                                                   return false;
                                               });

    }

    LinkedListLayer::LinkedListLayer(const std::string &name)
            : Layer(name)
    {
    }

    LinkedListLayer::~LinkedListLayer()
    {
        Destory();
    }

    void LinkedListLayer::OnAttach()
    {
        Init();
    }

    void LinkedListLayer::OnDetach()
    {
        Destory();
    }

    void LinkedListLayer::OnUpdate(Timestep ts)
    {
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_opaqueBackgroundColor));

        m_MatrixUniformBuffer->Bind();
        m_MatrixUniformBuffer->UpdateElementData("Color", glm::value_ptr(showColor_1));
        m_MatrixUniformBuffer->UnBind();

        m_TriangleShader->Bind();

        constexpr glm::mat4 model(1.0);
        m_TriangleShader->setMat4("model", model);
        m_CaseVertexArray->Bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_CaseVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);
    }

    void LinkedListLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings panel");
        ImGui::ColorEdit4("backgroundColor", glm::value_ptr(m_opaqueBackgroundColor), ImGuiColorEditFlags_NoAlpha);
        ImGui::ColorEdit4("showColor1", glm::value_ptr(showColor_1), ImGuiColorEditFlags_NoAlpha);
        ImGui::End();
    }

    void LinkedListLayer::Init()
    {
        m_TriangleShader = CreateRef<OpenGLShader>("./Shaders/LL_triangle.vert", "./Shaders/LL_triangle.frag");
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
        const UniformLayout layout = {
                        {ElementDataType::Matrix, "viewMat"},
                        {ElementDataType::Vec4, "Color"},
                        {ElementDataType::Matrix, "projectMat"},
        };

        m_MatrixUniformBuffer = UniformBuffer::Create(layout, 0);
        m_MatrixUniformBuffer->LinkBindingPoint();

        const CameraFunc cameraFunc(m_box, glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0));
        const auto view = glm::lookAt(cameraFunc.getPosition(), cameraFunc.getTarget(), glm::vec3(0, 1.0, 0.0));
        glm::vec3 min = m_box.min();
        glm::vec3 max = m_box.max();
        glm::mat4 projection = glm::ortho(min.x, max.x, min.y, max.y, 0.01f, 10.0f);

        m_MatrixUniformBuffer->Bind();
        m_MatrixUniformBuffer->UpdateElementData("viewMat", glm::value_ptr(view));
        m_MatrixUniformBuffer->UpdateElementData("projectMat", glm::value_ptr(projection));
        m_MatrixUniformBuffer->UpdateElementData("Color", glm::value_ptr(glm::vec4(1.0, 0.0, 0.0, 1.0)));
        m_MatrixUniformBuffer->UnBind();

        glCreateBuffers(1, &m_AtomicCounterBuffer);
        glNamedBufferData(m_AtomicCounterBuffer, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, m_AtomicCounterBuffer);

        InitLinkedListRes();
    }

    void LinkedListLayer::Destory()
    {
        m_CaseVertexArray.reset();
        m_TriangleShader.reset();
        m_LinkedNodeBuffer.reset();
        m_MatrixUniformBuffer.reset();
        m_HeadPointTexture.reset();
        m_LinkedNodeBuffer.reset();

        if(glIsBuffer(m_AtomicCounterBuffer))
        {
            glDeleteBuffers(1, &m_AtomicCounterBuffer);
        }
    }

    void LinkedListLayer::InitLinkedListRes()
    {
        m_HeadPointTexture.reset();
        m_LinkedNodeBuffer.reset();

        if (glIsBuffer(m_AtomicCounterBuffer))
        {
            glDeleteBuffers(1, &m_AtomicCounterBuffer);
        }

        /// init 
        const auto & currentWindow = Application::Get().GetWindow();
        const auto windowW = currentWindow.GetWidth();
        const auto windowH = currentWindow.GetHeight();

        const auto maxNodeCounts = windowW * windowH * MaxFragments;

        struct Node
        {
            glm::vec4 m_Color; /// fragment color
            float m_Depth;     /// fragment depth
            uint32_t m_Next;    /// fragment pointer
        };

        m_LinkedNodeBuffer = ShaderStorageBuffer::Create(sizeof(Node) * maxNodeCounts);
        m_LinkedNodeBuffer->ReLinkBindingPoint(1);

        m_HeadPointTexture = Texture::Create(static_cast<int>(windowW), static_cast<int>(windowH), 
            GL_RED_INTEGER, MultiSample::None);
        m_HeadPointTexture->Create();

        m_HeadPointTexture->Bind();
        m_HeadPointTexture->Clear(0, nullptr);
        m_HeadPointTexture->UnBind();
    }

    void LinkedListLayer::OnResizeEvent(const Event &event)
    {
        OBASE_INFO(event)
        InitLinkedListRes();
    }
} // OBase