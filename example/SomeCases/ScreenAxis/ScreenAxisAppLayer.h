#ifndef OPENGL_SCREENAXISAPPLAYER_H
#define OPENGL_SCREENAXISAPPLAYER_H

#include <glad/glad.h>
#include <RenderBase/Core/Layer.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "RenderBase/Render/VertexArray.h"
#include "RenderBase/OpenGL/OpenGLShader.h"

namespace OBase
{
    class ScreenAxisAppLayer : public OBase::Layer
    {
    public:
        explicit ScreenAxisAppLayer(const std::string & name = "Dual");
        ~ScreenAxisAppLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnEvent(Event &event) override;
        void OnImGuiRender() override;

    private:
        glm::vec4 m_BackgroundColor{0.2,0.3,0.4,1.0};
        float m_XRotate{0};
        float m_YRotate{0};
        float m_ZRotate{0};

        Ref<VertexArray> TriangleVertexArray;
        Ref<OpenGLShader> m_TriangleShader;

    };
}

#endif //OPENGL_SCREENAXISAPPLAYER_H
