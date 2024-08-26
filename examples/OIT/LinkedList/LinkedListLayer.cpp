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

#include "RenderBase/ToolFunc/OpenGLDebugger.h"

constexpr uint8_t MaxFragments = 20;

namespace OBase
{
    void LinkedListLayer::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](auto &&event)
                                               {
                                                   OnResizeEvent(std::forward<decltype(event)>(event));
                                                   return false; });
    }

    LinkedListLayer::LinkedListLayer(const std::string &name)
        : Layer(name)
    {
    }

    LinkedListLayer::~LinkedListLayer()
    {
        Destroy();
    }

    void LinkedListLayer::OnAttach()
    {
        Init();
        OpenGLDebugger::DisableDebug();
    }

    void LinkedListLayer::OnDetach()
    {
        Destroy();
    }

    void LinkedListLayer::OnUpdate(Timestep ts)
    {
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_backgroundColor));
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glDepthMask( GL_FALSE );

        // ClearBuffer();
        /// 初始化 原子计数器
//        GLuint zero = 0;
//        glNamedBufferSubData(m_AtomicCounterBuffer, 0, sizeof(GLuint), &zero);
//        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, m_AtomicCounterBuffer);

#if 1
        {
            GLuint zero = 0;
            glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, buffers[0] );
            glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);


            const auto &currentWindow = Application::Get().GetWindow();
            const auto windowW = currentWindow.GetWidth();
            const auto windowH = currentWindow.GetHeight();

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
            glBindTexture(GL_TEXTURE_2D, headPtrTex);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, windowW, windowH, GL_RED_INTEGER,
                            GL_UNSIGNED_INT, NULL);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }
#endif
        glm::mat4 model(1.0);

        m_CaseVertexArray->Bind();
        m_TriangleShader->Bind();

        m_TriangleShader->setMat4("model", model);
        m_TriangleShader->setVec4("uColor", showColor_1);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_CaseVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);

        model = glm::translate(model, glm::vec3(0.1, 0.0, 0.1));
        m_TriangleShader->setMat4("model", model);
        m_TriangleShader->setVec4("uColor", showColor_2);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_CaseVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);

        model = glm::translate(glm::mat4(1.0), glm::vec3(0.2, 0.0, 0.3));
        m_TriangleShader->setMat4("model", model);
        m_TriangleShader->setVec4("uColor", showColor_3);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_CaseVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);

        model = glm::translate(glm::mat4(1.0), glm::vec3(0.3, 0.0, 0.4));
        m_TriangleShader->setMat4("model", model);
        m_TriangleShader->setVec4("uColor", showColor_4);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_CaseVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);

        glFinish();

        glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        m_Render2ScreenShader->Bind();
        m_Render2ScreenShader->setVec4("backgroundColor", m_backgroundColor);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void LinkedListLayer::ClearBuffer()
    {
        /// 初始化 原子计数器
        GLuint zero = 0;
        glNamedBufferSubData(m_AtomicCounterBuffer, 0, sizeof(GLuint), &zero);
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, m_AtomicCounterBuffer);

        /// 初始化 头节点链表
        m_HeadPointTexture->Bind();
        GLuint cl = 0xffffffff;
        m_HeadPointTexture->Clear(0, &cl);
        m_HeadPointTexture->UnBind();
        m_HeadPointTexture->BindImage(2);
    }

    void LinkedListLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings panel");
        ImGui::ColorEdit4("backgroundColor", glm::value_ptr(m_backgroundColor), ImGuiColorEditFlags_NoAlpha);
        ImGui::ColorEdit4("showColor1", glm::value_ptr(showColor_1));
        ImGui::ColorEdit4("showColor2", glm::value_ptr(showColor_2));
        ImGui::ColorEdit4("showColor3", glm::value_ptr(showColor_3));
        ImGui::ColorEdit4("showColor4", glm::value_ptr(showColor_4));
        ImGui::End();
    }

    void LinkedListLayer::Init()
    {
        m_TriangleShader = CreateRef<OpenGLShader>("./Shaders/LL_triangle.vert", "./Shaders/LL_triangle.frag");
        m_Render2ScreenShader = CreateRef<OpenGLShader>("./Shaders/LL_renderScreen.vert", "./Shaders/LL_renderScreen.frag");

        m_box = BoundingBox(glm::dvec3(-2), glm::dvec3(2));

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

        InitUniformBuffer();
        // InitAtomicCounterBuffer();
//        InitLinkedListRes();

#if  1
        enum BufferNames
        {
            COUNTER_BUFFER = 0,
            LINKED_LIST_BUFFER
        };

        {
            const auto &currentWindow = Application::Get().GetWindow();
            const auto windowW = currentWindow.GetWidth();
            const auto windowH = currentWindow.GetHeight();

            glGenBuffers(2, buffers.data());
            GLuint maxNodes = MaxFragments * windowW * windowH;
            GLint nodeSize = 5 * sizeof(GLfloat) + sizeof(GLuint); // The size of a linked list node

            glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, buffers[BufferNames::COUNTER_BUFFER]);
            glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

            // The buffer for the head pointers, as an image texture
            glGenTextures(1, &headPtrTex);
            glBindTexture(GL_TEXTURE_2D, headPtrTex);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, windowW, windowH);
            glBindImageTexture(2, headPtrTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

            // The buffer of linked lists
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers[BufferNames::LINKED_LIST_BUFFER]);
            glBufferData(GL_SHADER_STORAGE_BUFFER, maxNodes * nodeSize, NULL, GL_DYNAMIC_DRAW);

            std::vector<GLuint> headPtrClearBuf(windowW*windowH, 0xffffffff);
            glGenBuffers(1, &clearBuf);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, clearBuf);
            glBufferData(GL_PIXEL_UNPACK_BUFFER, headPtrClearBuf.size() * sizeof(GLuint),
                         &headPtrClearBuf[0], GL_STATIC_COPY);

        }
#endif

    }

    void LinkedListLayer::InitUniformBuffer()
    {
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
    }

    void LinkedListLayer::InitAtomicCounterBuffer()
    {
        GLuint cval = 0;
        glCreateBuffers(1, &m_AtomicCounterBuffer);
        glNamedBufferStorage(m_AtomicCounterBuffer, sizeof(GLuint), &cval, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 3, m_AtomicCounterBuffer);
        // glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

    }

    void LinkedListLayer::Destroy()
    {
        m_CaseVertexArray.reset();
        m_TriangleShader.reset();
        m_LinkedNodeBuffer.reset();
        m_MatrixUniformBuffer.reset();
        m_HeadPointTexture.reset();
        m_LinkedNodeBuffer.reset();

        if (glIsBuffer(m_AtomicCounterBuffer))
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
        const auto &currentWindow = Application::Get().GetWindow();
        const auto windowW = currentWindow.GetWidth();
        const auto windowH = currentWindow.GetHeight();

        const auto maxNodeCounts = windowW * windowH * MaxFragments;

        struct Node
        {
            glm::vec4 m_Color; /// fragment color
            float m_Depth;     /// fragment depth
            uint32_t m_Next;   /// fragment pointer
        };

        m_LinkedNodeBuffer = ShaderStorageBuffer::Create(sizeof(Node) * maxNodeCounts);
        m_LinkedNodeBuffer->ReLinkBindingPoint(1);

        m_HeadPointTexture = Texture::Create(static_cast<int>(windowW), static_cast<int>(windowH),
                                             GL_R32UI, MultiSample::None);
        m_HeadPointTexture->Create();
        m_HeadPointTexture->BindImage(2);

        m_HeadPtrClearBuf = std::vector(windowW * windowH, 0xffffffff);
    }

    void LinkedListLayer::OnResizeEvent(const WindowResizeEvent &event)
    {
        OBASE_INFO(event)
        InitLinkedListRes();
        auto w = event.GetWidth();
        auto h = event.GetHeight();
        glViewport(0,0,w,h);
    }
} // OBase