#include "ABufferLayer.h"

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

#include "RenderBase/ToolFunc/OpenGLDebugger.h"

constexpr uint8_t MaxFragments = 20;

namespace OBase
{
    void ABufferLayer::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](auto &&event){
                                                   OnResizeEvent(std::forward<decltype(event)>(event));
                                                   return false; });
    }

    ABufferLayer::ABufferLayer(const std::string &name)
        : Layer(name)
    {
    }

    ABufferLayer::~ABufferLayer()
    {
        Destroy();
    }

    void ABufferLayer::OnAttach()
    {
        Init();
        OpenGLDebugger::DisableDebug();
    }

    void ABufferLayer::OnDetach()
    {
        Destroy();
    }

    void ABufferLayer::OnUpdate(Timestep ts)
    {
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_BackgroundColor));
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        ClearTex();
        renderScene(m_CaseTriangleShader, m_CaseVertexArray);

        glFinish();
        glMemoryBarrier( GL_TEXTURE_2D_ARRAY );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        m_DisplayABufferShader->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

    }

    void ABufferLayer::ClearTex() const
    {
        glClearTexImage(m_ABufferTexId, 0, GL_RGBA, GL_FLOAT, glm::value_ptr(m_AbufferClearValue));
        glClearTexImage(m_ABufferCounterTexId, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &m_ABufferCounterClearValue);
    }

    void ABufferLayer::renderScene(const Ref<OpenGLShader>& shader, const Ref<VertexArray>& vao) const
    {
        vao->Bind();
        shader->Bind();
        for(auto index = 0; index < m_Pos.size(); index++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0), m_Pos[index]);
            shader->setMat4("model", model);
            shader->setVec4("uColor", m_Colors[index]);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_CaseVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);
        }
    }

    void ABufferLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings panel");
        ImGui::ColorEdit4("backgroundColor", glm::value_ptr(m_BackgroundColor), ImGuiColorEditFlags_NoAlpha);

        for(auto index = 0; index < m_Pos.size(); index++)
        {
            ImGui::ColorEdit4(("showColor" + std::to_string(index)).c_str(), glm::value_ptr(m_Colors[index]),
                              ImGuiColorEditFlags_NoAlpha);
            ImGui::DragFloat3(("Position" + std::to_string(index)).c_str(), glm::value_ptr(m_Pos[index]),0.005, -1.0,
                              1.0);
        }

        ImGui::DragFloat("Global Alpha value: ", &m_Alpha, 0.001, 0.0, 1.0);
        ImGui::End();

        m_MatrixUniformBuffer->UpdateElementData("backgroundColor", glm::value_ptr(m_BackgroundColor));
        m_MatrixUniformBuffer->UpdateElementData("alpha", &m_Alpha);

    }

    void ABufferLayer::Init()
    {
        m_CaseTriangleShader = CreateRef<OpenGLShader>("./Shaders/ABuffer_triangle.vert", "./Shaders/ABuffer_triangle.frag");
        m_DisplayABufferShader = CreateRef<OpenGLShader>("./Shaders/DisplayABuffer.vert", "./Shaders/DisplayABuffer.frag");
        m_ClearAbufferShader = CreateRef<OpenGLShader>("./Shaders/ABuffer_triangle.vert", "./Shaders/ABuffer_clearAbuffer.frag");

        m_Box = BoundingBox(glm::dvec3(-2), glm::dvec3(2));

        m_CaseVertexArray = VertexArray::Create();
        {
            float vertices[3 * 7] =
                {
                    -.5f, -.5f, .0f, 0.0f, 0.2f, 0.8f, 1.0f,
                    .5f, -.5f, .0f, 0.2f, 0.4f, 0.8f, 1.0f,
                    .0f, +.5f, .0f, 0.8f, 0.8f, 0.2f, 1.0f};

            const auto vertexBuffer = (VertexBuffer::Create(vertices, sizeof(vertices)));

            vertexBuffer->Bind();
            {
                const BufferLayout layout = {
                    {ShaderDataType::Float3, "a_Position"},
                    {ShaderDataType::Float4, "a_Color"}};
                vertexBuffer->SetLayout(layout);
            }

            m_CaseVertexArray->AddVertexBuffer(vertexBuffer);

            uint32_t indices[3] = {0, 1, 2};
            const auto indexBuffer = (IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
            indexBuffer->Bind();

            m_CaseVertexArray->SetIndexBuffer(indexBuffer);
        }

        const auto & window = Application::Get().GetWindow();
        const auto windowSize = glm::vec2(window.GetWidth(), window.GetHeight());

        InitUniformBuffer(windowSize.x, windowSize.y);
        InitABufferTex(windowSize.x, windowSize.y);
    }

    void ABufferLayer::InitUniformBuffer(int w, int h)
    {
        const UniformLayout layout = {
            {ElementDataType::Matrix, "viewMat"},
            {ElementDataType::Matrix, "projectMat"},
            {ElementDataType::Vec2,"windowSize"},
            {ElementDataType::Vec4,"backgroundColor"},
            {ElementDataType::Float, "alpha"}
        };

        m_MatrixUniformBuffer = UniformBuffer::Create(layout, 0);
        m_MatrixUniformBuffer->LinkBindingPoint();

        const CameraFunc cameraFunc(m_Box, glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0));
        const auto view = glm::lookAt(cameraFunc.getPosition(), cameraFunc.getTarget(), glm::vec3(0, 1.0, 0.0));
        const glm::vec3 min = m_Box.min();
        const glm::vec3 max = m_Box.max();
        glm::mat4 projection = glm::ortho(min.x, max.x, min.y, max.y, 0.01f, 10.0f);

        auto windowSize = glm::vec2(w, h);

        m_MatrixUniformBuffer->Bind();
        m_MatrixUniformBuffer->UpdateElementData("viewMat", glm::value_ptr(view));
        m_MatrixUniformBuffer->UpdateElementData("projectMat", glm::value_ptr(projection));
        m_MatrixUniformBuffer->UpdateElementData("windowSize", glm::value_ptr(windowSize));
        m_MatrixUniformBuffer->UpdateElementData("backgroundColor", glm::value_ptr(m_BackgroundColor));
        m_MatrixUniformBuffer->UpdateElementData("alpha", &m_Alpha);

        m_MatrixUniformBuffer->UnBind();
    }

    void ABufferLayer::InitABufferTex(const unsigned int w, const unsigned int h)
    {
        if(glIsTexture(m_ABufferTexId))
        {
            glDeleteTextures(1, &m_ABufferTexId);
        }

        if (glIsTexture(m_ABufferCounterTexId))
        {
            glDeleteTextures(1, &m_ABufferCounterTexId);
        }

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_ABufferTexId);

        glTextureParameteri(m_ABufferTexId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_ABufferTexId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLint maxLayers = 0;
        glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
        if(maxLayers > MaxFragments)
        {
            maxLayers = MaxFragments;
        }

        glTextureStorage3D(m_ABufferTexId, 1, GL_RGBA32F, w, h, maxLayers);
        glBindImageTexture(1, m_ABufferTexId, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

        glCreateTextures(GL_TEXTURE_2D, 1 , &m_ABufferCounterTexId);
        glTextureStorage2D(m_ABufferCounterTexId, 1,  GL_R32UI, w, h);
        glTextureParameteri(m_ABufferCounterTexId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_ABufferCounterTexId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindImageTexture(2, m_ABufferCounterTexId, 0, false, 0, GL_READ_WRITE, GL_R32UI);
    }

    void ABufferLayer::Destroy()
    {
        m_CaseVertexArray.reset();
        m_CaseTriangleShader.reset();
        m_MatrixUniformBuffer.reset();
    }

    void ABufferLayer::OnResizeEvent(const WindowResizeEvent &event)
    {
        OBASE_INFO(event)
        const auto w = event.GetWidth();
        const auto h = event.GetHeight();
        InitABufferTex(w, h);

        /// update for perspective projection
        // auto projection = glm::perspective(glm::radians(45.0f), static_cast<float>(w) / h, 0.1f, 100.0f);
        // m_MatrixUniformBuffer->UpdateElementData("projectMat", glm::value_ptr(projection));
        {
            auto min = m_Box.min();
            auto max = m_Box.max();

            auto spanWidth = max.x - min.x;
            auto spanHeight = max.y - min.y;

            float orthoW = std::max(spanWidth, spanHeight);
            auto asptio = w / static_cast<float>(h);
            float orthH = orthoW / asptio;

            auto projection = glm::ortho(-orthoW / 2.0f, orthoW / 2.0f, -orthH / 2.0f, orthH / 2.0f, 0.01f, 100.0f);
            m_MatrixUniformBuffer->UpdateElementData("projectMat", glm::value_ptr(projection));
        }

        /// update view Port
        glViewport(0,0,static_cast<GLsizei>(w),static_cast<GLsizei>(h));
    }
} // OBase