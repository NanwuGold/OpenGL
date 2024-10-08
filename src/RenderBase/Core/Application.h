#ifndef OPENGL_APPLICATION_H
#define OPENGL_APPLICATION_H

#include <memory>
#include <string>

#include <RenderBase/Core/LayerStack.h>
#include <RenderBase/Core/Window.h>
#include <RenderBase/ImGui/ImGuiLayer.h>

namespace OBase
{
    class WindowCloseEvent;
    class WindowResizeEvent;
};

namespace OBase
{
    class Application
    {
    public:
        explicit Application(const std::string & name = "Render Window");

        Application(Application&) = delete;
        Application(const Application&&) = delete;
        Application & operator=(Application&) = delete;
        Application & operator=(const Application&&) = delete;

        /**
         * @brief render loop
         */
        void Run();

        /**
         * @brief 处理事件
         * @param e 传递的事件
         */
        void OnEvent(Event &e);

        /**
         * @brief 添加层
         * @param layer 当前需要添加的层
         */
        [[maybe_unused]] void PushLayer(const Ref<Layer> &layer);

        /**
         * @brief 添加overlay
         * @param overlay 当前需要添加的overlay
         */
        [[maybe_unused]] void PushOverLay(const Ref<Layer> &overlay);

        /**
         * @brief 当前的应用程序对象
         * @return const Application &
         */
        static const Application & Get()
        {
            return *s_Instance;
        }

        /**
         * @brief 获取当前应用程序对象窗口对象
         * @return Window
         */
        [[nodiscard]] Window & GetWindow() const
        {
            return *m_Window;
        }

        virtual ~Application();

    protected:
        virtual void OnCloseEvent(const WindowCloseEvent & event);
        virtual void OnResizeEvent(const WindowResizeEvent & event);

    private:
        std::shared_ptr<Window> m_Window;
        bool m_Running{true};
        float m_LastFrameTime{0.0f};
        LayerStack m_LayerStack;

        Ref<ImGuiLayer> m_imGuiLayer;



    private:
        struct ApplicationDeleter
        {
            void operator()([[maybe_unused]] Application *ptr) const
            {
            }
        };

        static std::unique_ptr<Application,ApplicationDeleter> s_Instance;  ///< 应用程序对象

    };


    std::unique_ptr<Application> CreateApplication();

} // OBase

#endif //OPENGL_APPLICATION_H
