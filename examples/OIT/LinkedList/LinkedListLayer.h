#ifndef OPENGL_DEPTHPEELINGLAYER_H
#define OPENGL_DEPTHPEELINGLAYER_H

#include <RenderBase/Core/Layer.h>
#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/Geometry/BoundingBox.h>
#include <RenderBase/Render/Texture.h>

#include <glm/glm.hpp>
#include <array>
#include "RenderBase/Event/ApplicationEvent.h"

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
        explicit LinkedListLayer(const std::string &name);

        LinkedListLayer(LinkedListLayer &) = delete;
        LinkedListLayer(const LinkedListLayer &&) = delete;
        LinkedListLayer &operator=(LinkedListLayer &) = delete;
        LinkedListLayer &operator=(const LinkedListLayer &&) = delete;

        ~LinkedListLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;

        void OnEvent(Event &event) override;

    protected:
        void Init();
        void Destroy();
        void InitLinkedListRes();

        void OnResizeEvent(const OBase::WindowResizeEvent &event);

        void InitAtomicCounterBuffer();
        void InitUniformBuffer();
        void InitHeadPointTexture(const uint32_t windowW, const uint32_t windowH);
        void ClearBuffer() const;

    private:
        glm::vec4 m_backgroundColor{0.2, 0.3, 0.4, 1.0};

        glm::vec4 showColor_1{0.0, 1.0, 0.0, 0.5};
        glm::vec4 showColor_2{1.0, 0.0, 0.0, 0.5};
        glm::vec4 showColor_3{0.0, 0.0, 1.0, 0.5};
        glm::vec4 showColor_4{1.0, 0.5, 0.0, 0.5};
        glm::vec4 showColor_5{0.6, 0.5, 0.0, 0.5};

        Ref<VertexArray> m_CaseVertexArray;
        Ref<OpenGLShader> m_TriangleShader;
        Ref<OpenGLShader> m_Render2ScreenShader;
        Ref<UniformBuffer> m_MatrixUniformBuffer;
        BoundingBox m_box{};

        Ref<Texture> m_HeadPointTexture;             ///< 链表头节点指针缓冲
        Ref<ShaderStorageBuffer> m_LinkedNodeBuffer; ///< 链表节点缓冲
        unsigned int m_AtomicCounterBuffer{0};
        std::vector<GLuint> m_HeadPtrClearBuf;
    };

}

#endif // OPENGL_DEPTHPEELINGLAYER_H
