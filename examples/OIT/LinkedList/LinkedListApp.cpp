#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>
#include <RenderBase/ToolFunc/OpenGLDebugger.h>

#include "LinkedListLayer.h"

namespace OBase
{
    class LinkedListApp final : public Application
    {
    public:
        LinkedListApp(LinkedListApp&) = delete;
        LinkedListApp(const LinkedListApp&&) = delete;
        LinkedListApp& operator=(LinkedListApp&) = delete;
        LinkedListApp& operator=(const LinkedListApp&&) = delete;

        explicit LinkedListApp(const std::string & name = "LinkedListApp")
                :Application(name)
        {
            /// 注册 debug 输出
            OpenGLDebugger::EnableDebug();

            const auto renderLayer = std::make_shared<LinkedListLayer>("LinkedListApp Render Layer");
             PushLayer(renderLayer);
        }

        ~LinkedListApp() override = default;

    protected:
        void OnCloseEvent(const WindowCloseEvent& event) override
        {
            Application::OnCloseEvent(event);
        }

        void OnResizeEvent(const WindowResizeEvent& event) override
        {
            Application::OnResizeEvent(event);
        }
    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<LinkedListApp>("LinkedListApp Application");
    }

}



