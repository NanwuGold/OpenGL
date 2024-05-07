#ifndef CSM_H_
#define CSM_H_

#include <RenderBase/Core/OGLBase.h>
#include <RenderBase/Event/ApplicationEvent.h>

#include <glad/glad.h>

#include "CSMLayer.h"

namespace OBase
{
    class CsmApp : public Application
    {
    public:

        CsmApp(CsmApp&) = delete;
        CsmApp(const CsmApp&&) = delete;
        CsmApp& operator=(CsmApp&) = delete;
        CsmApp& operator=(const CsmApp&&) = delete;


        CsmApp()
            :Application("CsmApp Window")
        {
            const auto RenderLayer = std::make_shared<CSMLayer>("CsmApp Render");
            PushLayer(RenderLayer);
        }

        ~CsmApp() override
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
        return std::make_unique<CsmApp>();
    }
}




#endif // !CSM_H_
