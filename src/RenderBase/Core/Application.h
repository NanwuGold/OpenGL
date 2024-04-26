#ifndef OPENGL_APPLICATION_H
#define OPENGL_APPLICATION_H

#include "RenderBase/Core/Window.h"

#include <string>
#include <memory>

namespace OBase
{
    class Application
    {
    public:
        explicit Application(const std::string & name = "Render Window");
        virtual void run();
        ~Application();

    private:
        std::shared_ptr<Window> m_Window;                                   /// app的窗口
        bool m_Running{true};
    };


    std::unique_ptr<Application> CreateApplication();

} // OBase

#endif //OPENGL_APPLICATION_H
