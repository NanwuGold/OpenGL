#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>

#include "EmptyCaseLayer.h"

namespace OBase
{
    class EmptyCaseApp final : public Application
    {
    public:
        EmptyCaseApp(EmptyCaseApp&) = delete;
        EmptyCaseApp(const EmptyCaseApp&&) = delete;
        EmptyCaseApp& operator=(EmptyCaseApp&) = delete;
        EmptyCaseApp& operator=(const EmptyCaseApp&&) = delete;

        explicit EmptyCaseApp(const std::string & name = "Empty Case")
                :Application(name)
        {
             const auto renderLayer = std::make_shared<EmptyCaseLayer>("EmptyCaseApp Render Layer");
             PushLayer(renderLayer);
        }

        ~EmptyCaseApp() override;

    protected:
        void OnCloseEvent(const WindowCloseEvent& event) override;
        void OnResizeEvent(const WindowResizeEvent& event) override;
    };

    EmptyCaseApp::~EmptyCaseApp() = default;

    void EmptyCaseApp::OnCloseEvent(const WindowCloseEvent& event)
    {
        Application::OnCloseEvent(event);
    }

    void EmptyCaseApp::OnResizeEvent(const WindowResizeEvent& event)
    {
        Application::OnResizeEvent(event);
    }

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<EmptyCaseApp>("EmptyCaseApp Application");
    }

}



