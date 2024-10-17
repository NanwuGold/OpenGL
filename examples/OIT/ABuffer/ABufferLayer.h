#ifndef OPENGL_DEPTHPEELINGLAYER_H
#define OPENGL_DEPTHPEELINGLAYER_H

#include <RenderBase/Core/Layer.h>
#include <RenderBase/pointer_ptr.hpp>
#include <RenderBase/Geometry/BoundingBox.h>
#include <RenderBase/Render/Texture.h>
#include "RenderBase/Event/ApplicationEvent.h"

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
    class ABufferLayer final : public Layer
    {
    public:
        explicit ABufferLayer(const std::string &name);

        ABufferLayer(ABufferLayer &) = delete;
        ABufferLayer(const ABufferLayer &&) = delete;
        ABufferLayer &operator=(ABufferLayer &) = delete;
        ABufferLayer &operator=(const ABufferLayer &&) = delete;

        ~ABufferLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;

        void OnEvent(Event &event) override;

    private:
        void Init();
        void Destroy();
        void InitUniformBuffer(int w, int h);

        void InitABufferTex(unsigned w, unsigned h);

    protected:
        void OnResizeEvent(const OBase::WindowResizeEvent &event);

    private:
        glm::vec4 m_BackgroundColor{0.2, 0.3, 0.4, 1.0};

        glm::vec4 m_ShowColor1{0.0, 1.0, 0.0, 0.5};
        glm::vec4 m_ShowColor2{1.0, 0.0, 0.0, 0.5};
        glm::vec4 m_ShowColor3{0.0, 0.0, 1.0, 0.5};
        glm::vec4 m_ShowColor4{1.0, 0.5, 0.0, 0.5};
        glm::vec4 m_ShowColor5{0.6, 0.5, 0.0, 0.5};

        Ref<VertexArray> m_CaseVertexArray;
        Ref<OpenGLShader> m_CaseTriangleShader;
        Ref<OpenGLShader> m_ClearAbufferShader;

        Ref<OpenGLShader> m_DisplayABufferShader;
        Ref<UniformBuffer> m_MatrixUniformBuffer;
        BoundingBox m_Box{};


        [[maybe_unused]] unsigned int m_ABufferTexId;
        [[maybe_unused]] unsigned int m_ABufferCounterTexId;
        [[maybe_unused]] unsigned int m_ABufferSemphoresTexId;

        std::vector<glm::vec3> m_Pos{{0.0, 0.0, 0.0},
                                     {0.1, 0.0, -0.1},
                                     {0.2, 0.0, -0.2},
                                     {0.3, 0.2, -0.15},
                                     {0.4, 0.0, -0.3}
                                     };

        std::vector<glm::vec4> m_Colors{
                {0.0, 1.0, 0.0, 0.5},
                {1.0, 0.0, 0.0, 0.5},
                {0.0, 0.0, 1.0, 0.6},
                {1.0, 0.5, 0.0, 0.5},
                {0.6, 0.5, 0.5, 0.5}
        };


        glm::vec4 m_AbufferClearValue{0.0};
        uint32_t m_ABufferCounterClearValue{0};
        float m_Alpha{0.5};

    private:
        void renderScene(const Ref <OpenGLShader>& shader, const Ref <VertexArray>& vao) const;

        void ClearTex() const;
    };

}

#endif // OPENGL_DEPTHPEELINGLAYER_H
