#ifndef OPENGL_DUALDEPTHPEELINGLAYER_H
#define OPENGL_DUALDEPTHPEELINGLAYER_H

#include <glad/glad.h>
#include <RenderBase/Core/Layer.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/OpenGL/OpenGLShader.h"

#include <RenderBase/Geometry/BoundingBox.h>
#include <RenderBase/OpenGL/OpenGLFrameBuffer.h>

namespace OBase
{
    class DualDepthPeelingLayer : public OBase::Layer
    {
    public:
        explicit DualDepthPeelingLayer(const std::string & name = "Dual");
        ~DualDepthPeelingLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnEvent(Event &event) override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_BackgroundColor{0.2,0.3,0.4,1.0};

        Ref<FrameBuffer> m_DualDepthPeelingFramebuffer;   ///< 用于剥离层
        Ref<FrameBuffer> m_BlendFramebuffer;              ///< 混合剥离的结果

        Ref<VertexArray> m_DataVertexArray;
        Ref<OpenGLShader> m_InitDualDepthShader;
        Ref<OpenGLShader> m_TriangleShader;

        BoundingBox m_box{};  ///< 场景的包围盒

        float m_rotateY{0};   ///< 旋转

    };
}

#endif //OPENGL_DUALDEPTHPEELINGLAYER_H
