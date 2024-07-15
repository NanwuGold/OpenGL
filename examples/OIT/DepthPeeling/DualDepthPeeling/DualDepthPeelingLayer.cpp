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
    int g_numPasses = 2;
}

namespace OBase
{
    DualDepthPeelingLayer::DualDepthPeelingLayer(const std::string &name)
            : OBase::Layer(name)
    {

    }

    DualDepthPeelingLayer::~DualDepthPeelingLayer()
    {

    }

    void DualDepthPeelingLayer::initDualSources()
    {
        const auto & window = OBase::Application::Get().GetWindow();
        int windowWidget = static_cast<int>(window.GetWidth());
        int windowHeight = static_cast<int>(window.GetHeight());

        m_DualDepthPeelingFramebuffer = FrameBuffer::Create();
        {
            for(auto i = 0; i < 2;i++)
            {
                m_DualDepthTextures[i] = Texture::Create(windowWidget,windowHeight,GL_RG32F,MultiSample::None);
                m_DualDepthTextures[i]->Create();

                m_DualFrontColorTextures[i] = Texture::Create(windowWidget,windowHeight,GL_RGBA8,MultiSample::None);
                m_DualFrontColorTextures[i]->Create();

                m_DualBackColorTextures[i] = Texture::Create(windowWidget,windowHeight,GL_RGBA8,MultiSample::None);
                m_DualBackColorTextures[i]->Create();
            }
        }

        m_BlendFramebuffer = FrameBuffer::Create();
        {
            auto blendDepth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                               MultiSample::None);
            blendDepth->Create();
            m_BlendTexture = Texture::Create(windowWidget, windowHeight, GL_RGBA8,
                                             MultiSample::None);
            m_BlendTexture->Create();
            m_BlendFramebuffer->Create({m_BlendTexture},blendDepth);
        }


        const auto depth = Texture::Create(windowWidget, windowHeight, GL_DEPTH24_STENCIL8,
                                           MultiSample::None);
        depth->Create();
        m_DualDepthPeelingFramebuffer->Create({m_DualDepthTextures[0],m_DualFrontColorTextures[0],
                                               m_DualBackColorTextures[0],m_DualDepthTextures[1],
                                               m_DualFrontColorTextures[1],
                                               m_DualBackColorTextures[1],m_BlendTexture},depth);

    }

    void DualDepthPeelingLayer::OnAttach()
    {
        initDualSources();
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
        m_InitDepthLayerShader = CreateRef<OpenGLShader>("./Shaders/dual_triangle.vert","./Shaders/dual_init_depth_frag.glsl");

        const auto &window = OBase::Application::Get().GetWindow();
        int windowWidget = static_cast<int>(window.GetWidth());
        int windowHeight = static_cast<int>(window.GetHeight());

        m_TriangleShader->Bind();
        m_TriangleShader->setVec2("screenSize", glm::vec2(windowWidget,windowHeight));

        m_box = BoundingBox(glm::dvec3(-2),glm::dvec3(2));
    }

    void DualDepthPeelingLayer::OnImGuiRender()
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("BackGround Color",glm::value_ptr(m_BackgroundColor),ImGuiColorEditFlags_NoAlpha);
        ImGui::SliderFloat("Camera rotate",&m_rotateY,-180,180);
        auto index = 0;
        for(auto & pos : m_translatePos)
        {
            auto label = std::string("Pos") + std::to_string(index++);
            ImGui::DragFloat3(label.c_str(),glm::value_ptr(pos),0.05,-1.0,1.0);
        }

        ImGui::Image(reinterpret_cast<void*>(m_DualDepthTextures[0]->RenderID()),ImVec2{200,160});
        ImGui::End();
    }

    void DualDepthPeelingLayer::OnUpdate(Timestep ts)
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        m_DualDepthPeelingFramebuffer->Bind();

        auto colorClearBuffer = glm::vec4(0.0,0.0,0.0,0.0);
        auto depthClearValue = glm::vec4(-1.0,-1.0,0.0,0.0);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(depthClearValue));
        glClearBufferfv(GL_COLOR, 1, glm::value_ptr(colorClearBuffer));
        glClearBufferfv(GL_COLOR, 2, glm::value_ptr(colorClearBuffer));
        glClearBufferfv(GL_COLOR, 6, glm::value_ptr(m_BackgroundColor));
        glBlendEquation(GL_MAX);

        /// -------------------------------------------------------------
        /// init first depth layer
        /// -------------------------------------------------------------
        glDrawBuffer(buffers[0]);
        m_InitDepthLayerShader->Bind();

        auto mat = glm::rotate(glm::mat4(1.0),glm::radians(m_rotateY),glm::vec3(0,1.0,0));
        CameraFunc cameraFunc(m_box, glm::vec3 (mat * glm::vec4(0,0,1.0,1.0)));
        auto view = glm::lookAt(cameraFunc.getPosition(), cameraFunc.getTarget(), glm::vec3(0,1.0,0.0));
        auto min = m_box.min();
        auto max = m_box.max();
        auto projection = glm::ortho(min.x,max.x,min.y,max.y,0.01,10.0);
        glm::mat4 model(1.0);

        m_InitDepthLayerShader->setMat4("model",model);
        m_InitDepthLayerShader->setMat4("view",view);
        m_InitDepthLayerShader->setMat4("projection",projection);
        m_DataVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, m_DataVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        for(const auto & pos : m_translatePos)
        {
            model = glm::translate(glm::mat4(1.0),pos);
            m_InitDepthLayerShader->setMat4("model",model);
            glDrawElements(GL_TRIANGLES, m_DataVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        /// -------------------------------------------------------------
        /// do peeling depth layer
        /// -------------------------------------------------------------

        auto currID = 0;
        for(int pass = 1; pass < g_numPasses; pass++)
        {
            /// 一共六个颜色缓冲 每次使用三个颜色缓冲分别记录当前被剥离层的深度 以及前后被剥离的颜色
            currID = pass % 2;    ///< 获取当前后方向被剥离的结果
            auto prevID = 1 - currID;   ///< 获取前一次记录的深度信息 在剥离的时候使用
            auto bufferID = currID * 3;  ///< 记录当前使用的颜色缓冲的起点 往后数三则为当前次剥离使用的颜色缓冲

            glDrawBuffers(buffers.size(), buffers.data());
            glClearBufferfv(GL_COLOR, bufferID, glm::value_ptr(depthClearValue));
            glClearBufferfv(GL_COLOR, bufferID + 1, glm::value_ptr(colorClearBuffer));
            glClearBufferfv(GL_COLOR, bufferID + 2, glm::value_ptr(colorClearBuffer));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, glm::value_ptr(m_BackgroundColor));
        m_TriangleShader->Bind();
        m_DataVertexArray->Bind();
        m_TriangleShader->setMat4("view",view);
        m_TriangleShader->setMat4("projection",projection);
        m_TriangleShader->setMat4("model",glm::mat4(1.0));
        glDrawElements(GL_TRIANGLES, m_DataVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void DualDepthPeelingLayer::OnEvent(Event &event)
    {
        Layer::OnEvent(event);
    }

    void DualDepthPeelingLayer::OnDetach()
    {
        //destory resources
    }
}