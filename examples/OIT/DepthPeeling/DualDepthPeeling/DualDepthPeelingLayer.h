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

        Ref<VertexArray> TriangleVertexArray;
        Ref<OpenGLShader> m_TriangleShader;

        int m_ZoomDelta{};

        BoundingBox m_box{};

        float m_rotateY{0};

    };
}

#endif //OPENGL_DUALDEPTHPEELINGLAYER_H
