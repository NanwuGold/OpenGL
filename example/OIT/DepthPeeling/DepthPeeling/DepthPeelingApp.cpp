#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>
#include <glad/glad.h>
#include "DepthPeelingLayer.h"

namespace OBase
{
    class DepthPeeling final : public Application
    {
    public:
        DepthPeeling(DepthPeeling&) = delete;
        DepthPeeling(const DepthPeeling&&) = delete;
        DepthPeeling& operator=(DepthPeeling&) = delete;
        DepthPeeling& operator=(const DepthPeeling&&) = delete;

        explicit DepthPeeling(const std::string & name = "Depth Peeling")
            :Application(name)
        {
            const auto renderLayer = std::make_shared<DepthPeelingLayer>("DepthPeeling Render");
            PushLayer(renderLayer);
        }

        ~DepthPeeling() override;

    protected:
        void OnCloseEvent(const WindowCloseEvent& event) override;
        void OnResizeEvent(const WindowResizeEvent& event) override;
    };

    DepthPeeling::~DepthPeeling() = default;

    void DepthPeeling::OnCloseEvent(const WindowCloseEvent& event)
    {
        Application::OnCloseEvent(event);
    }

    void DepthPeeling::OnResizeEvent(const WindowResizeEvent& event)
    {
        Application::OnResizeEvent(event);
    }



    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<DepthPeeling>("DepthPeeling Application");
    }

}



