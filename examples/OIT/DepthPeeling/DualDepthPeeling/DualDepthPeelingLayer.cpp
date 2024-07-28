#include "DualDepthPeelingLayer.h"
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/Core/Application.h"
#include <RenderBase/Render/Buffer.h>
#include <RenderBase/Render/FrameBuffer.h>
#include <RenderBase/Geometry/BoundingBox.h>
#include <RenderBase/Camera/CameraFunc.h>

#include <random>

namespace
{
    int g_numPasses = 4;
    GLuint g_queryId;
}

namespace OBase
{
    DualDepthPeelingLayer::DualDepthPeelingLayer(const std::string& name)
        : Layer(name)
    {
    }

    DualDepthPeelingLayer::~DualDepthPeelingLayer() = default;

    void DualDepthPeelingLayer::InitDualSources()
    {
        const auto& window = OBase::Application::Get().GetWindow();
        const int windowWidget = static_cast<int>(window.GetWidth());
        const int windowHeight = static_cast<int>(window.GetHeight());

        m_DualDepthPeelingFramebuffer = FrameBuffer::Create();
        {
            for (auto i = 0; i < 2; i++)
            {
                m_DualDepthTextures[i] = Texture::Create(windowWidget, windowHeight,GL_RG32F, MultiSample::None);
                m_DualDepthTextures[i]->Create();

                m_DualFrontColorTextures[i] = Texture::Create(windowWidget, windowHeight,GL_RGBA8, MultiSample::None);
                m_DualFrontColorTextures[i]->Create();

                m_DualBackColorTextures[i] = Texture::Create(windowWidget, windowHeight,GL_RGBA8, MultiSample::None);
                m_DualBackColorTextures[i]->Create();
            }
        }

        m_BlendFramebuffer = FrameBuffer::Create();
        {
            const auto blendDepth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                                    MultiSample::None);
            blendDepth->Create();
            m_BlendTexture = Texture::Create(windowWidget, windowHeight, GL_RGBA8,
                                             MultiSample::None);
            m_BlendTexture->Create();
            m_BlendFramebuffer->Create({m_BlendTexture}, blendDepth);
        }


        const auto depth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                           MultiSample::None);
        depth->Create();
        m_DualDepthPeelingFramebuffer->Create({
                                                  m_DualDepthTextures[0], m_DualFrontColorTextures[0],
                                                  m_DualBackColorTextures[0], m_DualDepthTextures[1],
                                                  m_DualFrontColorTextures[1],
                                                  m_DualBackColorTextures[1], m_BlendTexture
                                              }, depth);

        glGenQueries(1, &g_queryId);

    }

    void DualDepthPeelingLayer::RenderScene(OBase::Ref<OpenGLShader>& shader) const
    {
        auto model = glm::mat4(1.0);
        for (auto & [pos, color] : elements)
        {
            model = glm::translate(glm::mat4(1.0), pos);
            shader->setMat4("model", model);
            shader->setVec4("outColor", color);
            glDrawElements(GL_TRIANGLES, m_DataVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    void DualDepthPeelingLayer::OnAttach()
    {
        InitDualSources();
        m_DataVertexArray = VertexArray::Create();
        {
            float vertices[3 * 7] =
            {
                -.5f, -.5f, .0f, 0.0f, 0.2f, 0.8f, 0.5f,
                .5f, -.5f, .0f, 0.2f, 0.4f, 0.8f, 0.5f,
                .0f, +.5f, .0f, 0.8f, 0.8f, 0.2f, 0.5f
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

        m_PeelingFinalShader = CreateRef<OpenGLShader>("./Shaders/dual_peeling_final.vert", "./Shaders/dual_peeling_final.frag");

        m_InitDepthLayerShader = CreateRef<OpenGLShader>("./Shaders/dual_init_depth_vert.glsl",
                                                         "./Shaders/dual_init_depth_frag.glsl");
        m_PeelingLayerShader = CreateRef<OpenGLShader>("./Shaders/dual_peeling_peeling.vert",
                                                       "./Shaders/dual_peeling_peeling.frag");
        m_BlendBackPeelingBlendShader = CreateRef<OpenGLShader>("./Shaders/dual_blend_back.vert",
                                                                "./Shaders/dual_blend_back.frag");

        const auto& window = OBase::Application::Get().GetWindow();

        m_Box = BoundingBox(glm::dvec3(-2), glm::dvec3(2));

        m_PeelingFinalShader->Bind();
        m_PeelingFinalShader->setInt("FrontBlenderTex", 0);
        m_PeelingFinalShader->setInt("BackBlenderTex", 1);
        m_PeelingFinalShader->setInt("DepthBlenderTex", 2);

        m_BlendBackPeelingBlendShader->Bind();
        m_BlendBackPeelingBlendShader->setInt("screenTexture", 0);

        m_PeelingLayerShader->Bind();
        m_PeelingLayerShader->setInt("DepthTexture", 0);
        m_PeelingLayerShader->setInt("FrontColorTexture", 1);
        m_PeelingLayerShader->setVec2("viewSize", glm::vec2(window.GetWidth(),window.GetHeight()));
    }

    void DualDepthPeelingLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("BackGround Color", glm::value_ptr(m_BackgroundColor), ImGuiColorEditFlags_NoAlpha);
        ImGui::SliderFloat("Camera rotate", &m_RotateY, -180, 180);
        auto index = 0;
        for (auto & [pos,color] : elements)
        {
            auto label = std::string("Pos") + std::to_string(index);
            ImGui::DragFloat3(label.c_str(), glm::value_ptr(pos), 0.05f, -1.0f, 1.0f);
            label = std::string("Color") + std::to_string(index);
            ImGui::ColorEdit4(label.c_str(), glm::value_ptr(color));
            index++;
        }
        ImGui::End();
    }

    void DualDepthPeelingLayer::OnUpdate(Timestep ts)
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        m_DualDepthPeelingFramebuffer->Bind();

        glDrawBuffers(static_cast<GLsizei>(m_Buffers.size()), m_Buffers.data());
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_DepthClearValue));
        glClearBufferfv(GL_COLOR, 1, glm::value_ptr(m_ColorClearBuffer));
        glClearBufferfv(GL_COLOR, 2, glm::value_ptr(m_ColorClearBuffer));
        glClearBufferfv(GL_COLOR, 6, glm::value_ptr(m_BackgroundColor));
        glBlendEquation(GL_MAX);

        /// -------------------------------------------------------------
        /// init first depth layer
        /// -------------------------------------------------------------
        glDrawBuffer(m_Buffers[0]);
        m_InitDepthLayerShader->Bind();

        const CameraFunc cameraFunc(m_Box, glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 0.0));
        const auto view = glm::lookAt(cameraFunc.getPosition(), cameraFunc.getTarget(), glm::vec3(0, 1.0, 0.0));
        const auto min = m_Box.min();
        const auto max = m_Box.max();
        const auto projection = glm::ortho(min.x, max.x, min.y, max.y, 0.01, 10.0);

        m_InitDepthLayerShader->setMat4("view", view);
        m_InitDepthLayerShader->setMat4("projection", projection);

        m_DataVertexArray->Bind();
        RenderScene(m_InitDepthLayerShader);

        /// -------------------------------------------------------------
        /// do peeling depth layer
        /// -------------------------------------------------------------

        int outFrontIndex = 0;
        for (int pass = 1; pass < g_numPasses; pass++)
        {
            /// 一共六个颜色缓冲 每次使用三个颜色缓冲分别记录当前被剥离层的深度 以及前后被剥离的颜色
            const auto currentIndex = pass % 2; ///< 获取当前后方向被剥离的结果
            const auto prevIndex = 1 - currentIndex; ///< 获取前一次记录的深度信息 在剥离的时候使用
            const auto bufferIndex = currentIndex * 3; ///< 记录当前使用的颜色缓冲的起点 往后数三则为当前次剥离使用的颜色缓冲
            outFrontIndex = currentIndex;
            /// 清空缓存
            glDrawBuffers(static_cast<GLsizei>(m_Buffers.size()), m_Buffers.data());
            glClearBufferfv(GL_COLOR, bufferIndex, glm::value_ptr(m_DepthClearValue));
            glClearBufferfv(GL_COLOR, bufferIndex + 1, glm::value_ptr(m_ColorClearBuffer));
            glClearBufferfv(GL_COLOR, bufferIndex + 2, glm::value_ptr(m_ColorClearBuffer));

            /// 指定绘制的buffer
            glDrawBuffers(3, &m_Buffers[bufferIndex]);
            glBlendEquation(GL_MAX);  ///< 每个分量都需要比较最大值

            m_DataVertexArray->Bind();
            m_PeelingLayerShader->Bind();

            /// 绑定纹理 上一帧剥离的深度范围
            glActiveTexture(GL_TEXTURE0);
            m_DualDepthTextures[prevIndex]->Bind();
            glActiveTexture(GL_TEXTURE1);
            m_DualFrontColorTextures[prevIndex]->Bind();

            m_PeelingLayerShader->setMat4("view", view);
            m_PeelingLayerShader->setMat4("projection", projection);
            RenderScene(m_PeelingLayerShader);

            /// --------------------------------- ----------------------------
            /// blend peeling back layer
            /// -------------------------------------------------------------

            glBeginQuery(GL_SAMPLES_PASSED,g_queryId);

            glDrawBuffer(m_Buffers.back());
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            m_BlendBackPeelingBlendShader->Bind();
            glActiveTexture(GL_TEXTURE0);
            m_DualBackColorTextures[currentIndex]->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glEndQuery(GL_SAMPLES_PASSED);
            GLuint sampleCount;
            glGetQueryObjectuiv(g_queryId,GL_QUERY_RESULT,&sampleCount);
            if(!sampleCount)
            {
                break;
            }
        }
        /// 合并前向剥离的结果与后向剥离的结果
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_BLEND);
        m_PeelingFinalShader->Bind();
        glActiveTexture(GL_TEXTURE0);
        m_DualFrontColorTextures[outFrontIndex]->Bind();
        glActiveTexture(GL_TEXTURE1);
        m_BlendTexture->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void DualDepthPeelingLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }

    void DualDepthPeelingLayer::OnDetach()
    {
        //destory resources
    }
}