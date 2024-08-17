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
    class ShaderStorageBuffer;
}

class OpenGLShader;

namespace OBase
{
    class LinkedListLayer final : public Layer
     {
    public:
        explicit LinkedListLayer(const std::string& name);

        LinkedListLayer(LinkedListLayer&) = delete;
        LinkedListLayer(const LinkedListLayer&&) = delete;
        LinkedListLayer& operator=(LinkedListLayer&) = delete;
        LinkedListLayer& operator=(const LinkedListLayer&&) = delete;

        ~LinkedListLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;

        void OnEvent(Event& event) override;

    protected:
        void Init();
        void Destory();

        void OnResizeEvent(const Event &event);
    private:
        glm::vec4 m_opaqueBackgroundColor{0.2,0.3,0.4,1.0};
        glm::vec4 showColor_1{0.0,1.0,0.0,1.0};
        Ref<VertexArray> m_CaseVertexArray;
        Ref<OpenGLShader> m_TriangleShader;
        Ref<UniformBuffer> m_MatrixUniformBuffer;
        Ref<ShaderStorageBuffer> m_ShaderStorageBufferBuffer;
        BoundingBox m_box{};
    };

}

#endif //OPENGL_DEPTHPEELINGLAYER_H
