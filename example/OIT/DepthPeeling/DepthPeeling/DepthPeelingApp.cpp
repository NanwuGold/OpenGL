#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>
#include <glad/glad.h>
#include "DepthPeelingLayer.h"

namespace OBase
{
    class DepthPeeling : public Application
    {
    public:
        DepthPeeling(const std::string & name = "Depth Peeling")
            :Application(name)
        {
            const auto RenderLayer = std::make_shared<DepthPeelingLayer>("DepthPeeling Render");
            PushLayer(RenderLayer);
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



