#ifndef OPENGL_DUALDEPTHPEELINGLAYER_H
#define OPENGL_DUALDEPTHPEELINGLAYER_H

#include <glad/glad.h>
#include <RenderBase/Core/Layer.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <RenderBase/Render/VertexArray.h>
#include <RenderBase/OpenGL/OpenGLShader.h>

#include <RenderBase/Geometry/BoundingBox.h>
#include <RenderBase/Render/FrameBuffer.h>
#include <RenderBase/Render/Texture.h>

#include <array>

namespace OBase
{
    class DualDepthPeelingLayer : public OBase::Layer
    {
    public:
        explicit DualDepthPeelingLayer(const std::string &name = "Dual");
        ~DualDepthPeelingLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnEvent(Event &event) override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_BackgroundColor{0.2, 0.3, 0.4, 1.0};

        Ref<FrameBuffer> m_DualDepthPeelingFramebuffer; ///< 用于剥离层

        std::array<Ref<Texture>, 2> m_DualDepthTextures;
        std::array<Ref<Texture>, 2> m_DualFrontColorTextures;
        std::array<Ref<Texture>, 2> m_DualBackColorTextures;

        Ref<FrameBuffer> m_BlendFramebuffer; ///< 混合剥离的结果
        Ref<Texture> m_BlendTexture;

        Ref<VertexArray> m_DataVertexArray;
        Ref<OpenGLShader> m_InitDualDepthShader;
        Ref<OpenGLShader> m_TriangleShader;

        Ref<OpenGLShader> m_InitDepthLayerShader;

        BoundingBox m_box{}; ///< 场景的包围盒
        float m_rotateY{0};  ///< 旋转

        std::array<GLenum, 7> buffers{GL_COLOR_ATTACHMENT0,  /// depth1
                                      GL_COLOR_ATTACHMENT1,  /// color1  front peeling 1
                                      GL_COLOR_ATTACHMENT2,  /// color2  back  peeling 1
                                      GL_COLOR_ATTACHMENT3,  /// depth2
                                      GL_COLOR_ATTACHMENT4,  /// color3  front peeling 2
                                      GL_COLOR_ATTACHMENT5,  /// color4  back  peeling 2
                                      GL_COLOR_ATTACHMENT6}; /// blend back color buffer

        std::array<glm::vec3, 4> m_translatePos{glm::vec3{-0.2, 0.3, 0.1}, glm::vec3{-0.1, 0.2, 0.2},
                                                glm::vec3{-0.0, 0.1, 0.0}, glm::vec3{0.1, 0.0, 0.0}};

    private:
        void initDualSources();
    };
}

#endif // OPENGL_DUALDEPTHPEELINGLAYER_H
