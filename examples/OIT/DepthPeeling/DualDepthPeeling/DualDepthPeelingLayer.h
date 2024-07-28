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
#include <vector>

namespace OBase
{
    class DualDepthPeelingLayer final : public OBase::Layer
    {
    public:
        explicit DualDepthPeelingLayer(const std::string &name = "Dual");
        ~DualDepthPeelingLayer() override;

        DualDepthPeelingLayer(const DualDepthPeelingLayer&) = delete;
        DualDepthPeelingLayer(DualDepthPeelingLayer&&) = delete;

        DualDepthPeelingLayer& operator=(const DualDepthPeelingLayer&) = delete;
        DualDepthPeelingLayer& operator=(DualDepthPeelingLayer&&) = delete;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnEvent(Event &event) override;
        void OnImGuiRender() override;

    protected:

        void RenderScene(Ref<OpenGLShader> & shader) const;

    private:
        glm::vec4 m_BackgroundColor{0.2, 0.3, 0.4, 1.0};
        glm::vec4 m_ColorClearBuffer{ 0.0, 0.0, 0.0, 0.0 };
        glm::vec4 m_DepthClearValue{ -1.0, -1.0, 0.0, 0.0 };

        BoundingBox m_Box{}; ///< 场景的包围盒
        float m_RotateY{ 0 };  ///< 旋转

        Ref<FrameBuffer> m_DualDepthPeelingFramebuffer; ///< 用于剥离层

        std::array<Ref<Texture>, 2> m_DualDepthTextures;
        std::array<Ref<Texture>, 2> m_DualFrontColorTextures;
        std::array<Ref<Texture>, 2> m_DualBackColorTextures;

        Ref<FrameBuffer> m_BlendFramebuffer; ///< 混合剥离的结果
        Ref<Texture> m_BlendTexture;

        /// Vertex Array
        Ref<VertexArray> m_DataVertexArray;

        /// Shaders
        Ref<OpenGLShader> m_InitDepthLayerShader;   ///< 第一次渲染 获取剥离的深度(最外层与最内层)
        Ref<OpenGLShader> m_PeelingLayerShader;     ///< 剥离前后层使用的shader
        Ref<OpenGLShader> m_PeelingFinalShader;   ///< 绘制三角形的着色器
        Ref<OpenGLShader> m_BlendBackPeelingBlendShader;   ///< 混合剥离的结果的shader

        std::array<GLenum, 7> m_Buffers{GL_COLOR_ATTACHMENT0,  /// depth1
                                      GL_COLOR_ATTACHMENT1,  /// color1  front peeling 1
                                      GL_COLOR_ATTACHMENT2,  /// color2  back  peeling 1
                                      GL_COLOR_ATTACHMENT3,  /// depth2
                                      GL_COLOR_ATTACHMENT4,  /// color3  front peeling 2
                                      GL_COLOR_ATTACHMENT5,  /// color4  back  peeling 2
                                      GL_COLOR_ATTACHMENT6}; /// blend back color buffer

        struct ElementAttribute
        {
            glm::vec3 position;
            glm::vec4 color;
        };

        std::vector<ElementAttribute> elements{{glm::vec3{-0.3, 0.4, 0.0},glm::vec4(1.0,0.0,0.0,0.5)},
                                               {glm::vec3{-0.2, 0.3, 0.1},glm::vec4(0.0,1.0,0.0,0.5)},
                                               {glm::vec3{-0.1, 0.2, 0.2},glm::vec4(0.0,0.0,1.0,0.5)},
                                               {glm::vec3{0.0, 0.1, 0.3},glm::vec4(0.5,0.0,0.5,0.5)},
                                               {glm::vec3{0.1, 0.0, 0.4},glm::vec4(0.0,0.5,0.5,0.5)}};


    private:
        void InitDualSources();

    };
}

#endif // OPENGL_DUALDEPTHPEELINGLAYER_H
