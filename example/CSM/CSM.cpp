#ifndef CSM_H_
#define CSM_H_

#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>

#include <glad/glad.h>

#include "CSMLayer.h"

namespace OBase
{
    class CSM : public Application
    {
    public:
        CSM()
            :Application("CSM Window")
        {
            const auto RenderLayer = std::make_shared<CSMLayer>("CSM Render");
            PushLayer(RenderLayer);
        }

        ~CSM() override
        {

        }

    protected:
        void OnCloseEvent(const WindowCloseEvent &event) override
        {
            Application::OnCloseEvent(event);
        }

        void OnResizeEvent(const WindowResizeEvent &event) override
        {
            glViewport(0,0,event.GetWidth(),event.GetHeight());
        }
    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<CSM>();
    }
}




#endif // !CSM_H_
