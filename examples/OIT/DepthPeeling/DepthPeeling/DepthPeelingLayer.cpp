#include "DepthPeelingLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <RenderBase/OpenGL/OpenGLShader.h>
#include <RenderBase/Render/Buffer.h>
#include <RenderBase/Render/FrameBuffer.h>
#include <RenderBase/Render/VertexArray.h>
#include <RenderBase/Core/Application.h>
#include <imgui.h>

#include <array>

namespace OBase
{
    void DepthPeelingLayer::OnEvent(Event &event)
    {
        Layer::OnEvent(event);
    }

    DepthPeelingLayer::DepthPeelingLayer(const std::string &name)
            : Layer(name)
    {
    }

    DepthPeelingLayer::~DepthPeelingLayer()
    {
        m_TriangleShader.reset();
        m_TriangleVertexArray.reset();
    }

    void DepthPeelingLayer::OnAttach()
    {
        m_PeelingShader = CreateRef<OpenGLShader>("./Shaders/Dp_peeling.vert", "./Shaders/Dp_peeling.frag");
        m_TriangleShader = CreateRef<OpenGLShader>("./Shaders/Dp_triangle.vert", "./Shaders/Dp_triangle.frag");
        m_blendShader = CreateRef<OpenGLShader>("./Shaders/blend.vert", "./Shaders/blend.frag");

        m_render2ScreenShader = CreateRef<OpenGLShader>("./Shaders/renderScreen.vert","./Shaders/renderScreen.frag");

        m_TriangleVertexArray = VertexArray::Create();
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

            m_TriangleVertexArray->AddVertexBuffer(vertexBuffer);

            uint32_t indices[3] = {0, 1, 2};
            const auto indexBuffer = (IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
            indexBuffer->Bind();

            m_TriangleVertexArray->SetIndexBuffer(indexBuffer);
        }
        const auto &window = OBase::Application::Get().GetWindow();

        int windowWidget = static_cast<int>(window.GetWidth());
        int windowHeight = static_cast<int>(window.GetHeight());
         
        m_firstFramebuffer = FrameBuffer::Create();
        {
            auto color = Texture::Create(windowWidget, windowHeight, GL_RGBA8, MultiSample::None);
            color->Create();

            const auto depth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                               MultiSample::None);
            depth->Create();

            m_firstFramebuffer->Create({color}, depth);
        }

        m_secondFramebuffer = FrameBuffer::Create();
        {
            const auto color = Texture::Create(windowWidget, windowHeight, GL_RGBA8, MultiSample::None);
            color->Create();

            const auto depth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                               MultiSample::None);
            depth->Create();

            m_secondFramebuffer->Create({color}, depth);
        }

        m_Blend1Framebuffer = FrameBuffer::Create();
        {
            const auto color0 = Texture::Create(windowWidget, windowHeight, GL_RGBA8, MultiSample::None);
            color0->Create();

            const auto depth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                               MultiSample::None);

            m_Blend1Framebuffer->Create({color0}, depth);
        }

        m_Blend2Framebuffer = FrameBuffer::Create();
        {
            const auto color0 = Texture::Create(windowWidget, windowHeight, GL_RGBA8, MultiSample::None);
            color0->Create();

            const auto depth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                               MultiSample::None);

            m_Blend2Framebuffer->Create({color0}, depth);
        }


        // 初始化遮挡查询
        glGenQueries(1, &m_DepthPeelingQuery);

        m_PeelingShader->Bind();
        m_PeelingShader->setInt("depthTex", 0);
        m_PeelingShader->setVec2("wSize", glm::vec2(window.GetWidth(), window.GetHeight()));

        m_render2ScreenShader->Bind();
        m_render2ScreenShader->setInt("screenTexture",0);

        m_blendShader->Bind();
        m_blendShader->setInt("inTex1",0);
        m_blendShader->setInt("inTex2",1);

    }

    void DepthPeelingLayer::OnDetach()
    {
        m_TriangleVertexArray.reset();
        m_TriangleShader.reset();
    }

    void DepthPeelingLayer::OnUpdate(Timestep ts)
    {
        ///渲染整个场景 获得场景的最前一层的深度喝颜色
        m_firstFramebuffer->Bind();
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_opaqueBackgroundColor));
        glClearBufferfv(GL_DEPTH, 0, &m_defaultClearDepthVal);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        m_TriangleVertexArray->Bind();
        m_TriangleShader->Bind();
        DrawScene(m_TriangleShader); ///< 获取最小深度

        auto currentBlendBuffer = m_firstFramebuffer;

        if (m_useDepthPeeling)
        {
            /// 开始遮挡查询
            std::array<Ref<FrameBuffer>, 2> depthPeelingFrameBuffers{m_firstFramebuffer, m_secondFramebuffer};
            std::array<Ref<FrameBuffer>, 2> blendFrameBuffers{m_Blend1Framebuffer, m_Blend2Framebuffer};
            currentBlendBuffer = m_Blend1Framebuffer;
            std::array<Ref<Texture>, 2> blendTextures{m_firstFramebuffer->GetAttachment(FramebufferAttachment::Color0),
                                                      m_secondFramebuffer->GetAttachment(
                                                              FramebufferAttachment::Color0)};

            for (auto i = 1; i < (m_PeelingNums - 1) * 2; i++)
            {
                auto currentID = i % 2;
                auto preID = 1 - currentID;

                depthPeelingFrameBuffers[currentID]->Bind();
                glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0)));
                glClear(GL_DEPTH_BUFFER_BIT);

                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);

                glBeginQuery(GL_SAMPLES_PASSED, m_DepthPeelingQuery);

                m_PeelingShader->Bind();
                glActiveTexture(GL_TEXTURE0);
                depthPeelingFrameBuffers[preID]->GetAttachment(FramebufferAttachment::Depth)->Bind();
                DrawScene(m_PeelingShader);

                glEndQuery(GL_SAMPLES_PASSED);

                /// 获取遮挡查询结果
                GLuint sampleCount;
                glGetQueryObjectuiv(m_DepthPeelingQuery, GL_QUERY_RESULT, &sampleCount);

                if (sampleCount < 1)
                {
                    break;
                }

                currentBlendBuffer = blendFrameBuffers[preID];

                if (i == 1)
                {
                    blendTextures[0] = depthPeelingFrameBuffers[0]->GetAttachment(FramebufferAttachment::Color0);
                    blendTextures[1] = depthPeelingFrameBuffers[1]->GetAttachment(FramebufferAttachment::Color0);
                } else
                {
                    blendTextures[0] = blendFrameBuffers[currentID]->GetAttachment(FramebufferAttachment::Color0);
                    blendTextures[1] = depthPeelingFrameBuffers[currentID]->GetAttachment(
                            FramebufferAttachment::Color0);
                }

                /// 合并结果
                currentBlendBuffer->Bind();
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);

                m_blendShader->Bind();
                glActiveTexture(GL_TEXTURE0);
                blendTextures[0]->Bind();
                glActiveTexture(GL_TEXTURE1);
                blendTextures[1]->Bind();

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, value_ptr(m_opaqueBackgroundColor));
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_render2ScreenShader->Bind();
        glActiveTexture(GL_TEXTURE0);
        currentBlendBuffer->GetAttachment(FramebufferAttachment::Color0)->Bind();
        glDrawArrays(GL_TRIANGLES,0,6);
    }

    void DepthPeelingLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings panel");

        ImGui::ColorEdit4("backgroundColor", glm::value_ptr(m_opaqueBackgroundColor), ImGuiColorEditFlags_Float |
                                                                                      ImGuiColorEditFlags_NoAlpha);

        static float speed = 0.01f;
        ImGui::DragFloat3("Red Pos", glm::value_ptr(m_redTrianglePos), speed, -1.0, 1.0);
        ImGui::DragFloat3("Green Pos", glm::value_ptr(m_greenTrianglePos), speed, -1.0, 1.0);
        ImGui::DragFloat3("Blue Pos", glm::value_ptr(m_blueTrianglePos), speed, -1.0, 1.0);

        ImGui::Checkbox("Use DepthPeeling",&m_useDepthPeeling);
        ImGui::SliderInt("Peeling Nums",&m_PeelingNums, 2, 8, "%d");

#if 0
        const char *items[] = {"OpaQue Color", "OpaQue Depth", "Peeling Color", "Peeling Depth", "Blend Color"};
        static int itemCurrentIdx = 4; // 当前选中的索引

        ImGui::Combo("show Texture", &itemCurrentIdx, items, IM_ARRAYSIZE(items));

        const std::unordered_map<std::string, GLuint> textures{
                {std::string(items[0]), m_firstFramebuffer->GetAttachment(FramebufferAttachment::Color0)->RenderID()},
                {std::string(items[1]), m_firstFramebuffer->GetAttachment(FramebufferAttachment::Depth)->RenderID()},
                {std::string(items[2]), m_secondFramebuffer->GetAttachment(FramebufferAttachment::Color0)->RenderID()},
                {std::string(items[3]), m_secondFramebuffer->GetAttachment(FramebufferAttachment::Depth)->RenderID()},
                {std::string(items[4]), m_Blend1Framebuffer->GetAttachment(FramebufferAttachment::Color0)->RenderID()}
        };

        const auto showImage = textures.at(std::string(items[itemCurrentIdx]));
        ImGui::Image(reinterpret_cast<void *>(showImage), ImVec2(160, 120),ImVec2(0,1),ImVec2(1,0));
#endif
        const ImGuiIO &io = ImGui::GetIO();
        (void) io;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::End();
    }

    void DepthPeelingLayer::DrawScene(const Ref<OpenGLShader>& shader)
    {
        const auto view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0),
                                      glm::vec3(0.0, 1.0, 0.0));

        const auto projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, 0.001, 100.0);

        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        shader->setMat4("model", glm::translate(glm::mat4(1.0), m_redTrianglePos));
        shader->setVec4("oColor", glm::vec4(1.0, 0.0, 0.0, 0.7));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_TriangleVertexArray->GetIndexBuffer()->GetCount()),
                       GL_UNSIGNED_INT, nullptr);

        shader->setMat4("model", glm::translate(glm::mat4(1.0), m_greenTrianglePos));
        shader->setVec4("oColor", glm::vec4(0.0, 1.0, 0.0, 0.5));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_TriangleVertexArray->GetIndexBuffer()->GetCount()),
                       GL_UNSIGNED_INT, nullptr);

        shader->setMat4("model", glm::translate(glm::mat4(1.0), m_blueTrianglePos));
        shader->setVec4("oColor", glm::vec4(0.0, 0.0, 1.0, 0.5));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_TriangleVertexArray->GetIndexBuffer()->GetCount()),
                       GL_UNSIGNED_INT, nullptr);

    }
} // OBase