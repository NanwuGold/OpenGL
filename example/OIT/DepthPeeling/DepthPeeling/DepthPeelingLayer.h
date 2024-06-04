#ifndef OPENGL_DEPTHPEELINGLAYER_H
#define OPENGL_DEPTHPEELINGLAYER_H

#include <glad/glad.h>
#include <RenderBase/Core/Layer.h>
#include <RenderBase/pointer_ptr.hpp>


#include <glm/glm.hpp>

class OpenGLShader;
class FrameBuffer;

namespace OBase
{
    class VertexArray;
}

namespace OBase
{
    class DepthPeelingLayer final : public Layer
    {
    public:
        void OnEvent(Event& event) override;

        DepthPeelingLayer(DepthPeelingLayer&) = delete;
        DepthPeelingLayer(const DepthPeelingLayer&&) = delete;
        DepthPeelingLayer& operator=(DepthPeelingLayer&) = delete;
        DepthPeelingLayer& operator=(const DepthPeelingLayer&&) = delete;

        explicit DepthPeelingLayer(const std::string& name);

        ~DepthPeelingLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;

        void OnImGuiRender() override;

    private:
        void DrawScene(Ref <OpenGLShader> shader);


    private:
        Ref<VertexArray> m_TriangleVertexArray;
        Ref<OpenGLShader> m_TriangleShader;
        Ref<OpenGLShader> m_blendShader;
        Ref<OpenGLShader> m_PeelingShader;
        Ref<OpenGLShader> m_render2ScreenShader;

        glm::vec4 m_opaqueBackgroundColor{0.3f, 0.4f, 0.5f, 1.0f};
        glm::vec3 m_redTrianglePos{0.0f,0.0f,0.0f};
        glm::vec3 m_greenTrianglePos{0.1f,0.0f,0.1f};
        glm::vec3 m_blueTrianglePos{0.2f,0.0f,0.0f};

        Ref<FrameBuffer> m_firstFramebuffer;
        Ref<FrameBuffer> m_secondFramebuffer;
        Ref<FrameBuffer> m_Blend1Framebuffer;
        Ref<FrameBuffer> m_Blend2Framebuffer;

        float m_defaultClearDepthVal{1.0};
        GLuint m_DepthPeelingQuery;

        bool m_useDepthPeeling{false};
        int m_PeelingNums{6};
    };

}

#endif //OPENGL_DEPTHPEELINGLAYER_H
