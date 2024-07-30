#ifndef OPENGL_DEPTHPEELINGLAYER_H
#define OPENGL_DEPTHPEELINGLAYER_H

#include <RenderBase/Core/Layer.h>
#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/Geometry/BoundingBox.h>

#include <glm/glm.hpp>

namespace OBase
{
    class VertexArray;
    class UniformBuffer;
}

class OpenGLShader;

namespace OBase
{
    class UniformBufferLayer final : public Layer
     {
    public:
        explicit UniformBufferLayer(const std::string& name);

        UniformBufferLayer(UniformBufferLayer&) = delete;
        UniformBufferLayer(const UniformBufferLayer&&) = delete;
        UniformBufferLayer& operator=(UniformBufferLayer&) = delete;
        UniformBufferLayer& operator=(const UniformBufferLayer&&) = delete;

        void OnEvent(Event& event) override;

        ~UniformBufferLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;

    protected:
        void Init();
        void Destory();

    private:
        glm::vec4 m_opaqueBackgroundColor{0.2,0.3,0.4,1.0};
        glm::vec4 showColor_1{0.0,1.0,0.0,1.0};
        Ref<VertexArray> m_CaseVertexArray;
        Ref<OpenGLShader> m_TriangleShader;
        Ref<UniformBuffer> m_MatrixUniformBuffer;
        BoundingBox m_box{};
        
    };

}

#endif //OPENGL_DEPTHPEELINGLAYER_H
