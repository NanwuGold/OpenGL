#ifndef OPENGL_DEPTHPEELINGLAYER_H
#define OPENGL_DEPTHPEELINGLAYER_H

#include <RenderBase/Core/Layer.h>
#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/Geometry/BoundingBox.h>

#include <glm/glm.hpp>

namespace OBase
{
    class VertexArray;
}

class OpenGLShader;

namespace OBase
{
    class EmptyCaseLayer final : public Layer
     {
    public:
        explicit EmptyCaseLayer(const std::string& name);

        EmptyCaseLayer(EmptyCaseLayer&) = delete;
        EmptyCaseLayer(const EmptyCaseLayer&&) = delete;
        EmptyCaseLayer& operator=(EmptyCaseLayer&) = delete;
        EmptyCaseLayer& operator=(const EmptyCaseLayer&&) = delete;

        void OnEvent(Event& event) override;

        ~EmptyCaseLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;

    protected:
        void Init();
        void Destory();

    private:
        glm::vec4 m_opaqueBackgroundColor{0.2,0.3,0.4,1.0};
        Ref<VertexArray> m_CaseVertexArray;
        Ref<OpenGLShader> m_TriangleShader;
        BoundingBox m_box{};

    };

}

#endif //OPENGL_DEPTHPEELINGLAYER_H
