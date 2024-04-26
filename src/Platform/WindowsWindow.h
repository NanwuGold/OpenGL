#ifndef OPENGL_WINDOWSWINDOW_H
#define OPENGL_WINDOWSWINDOW_H

#include <RenderBase/Core/Window.h>

struct GLFWwindow;

namespace OBase
{
    class OpenGLContext;
}

namespace OBase
{
    class WindowsWindow final : public Window
    {
    public:
        explicit WindowsWindow(const OBase::WindowProps & props);
        WindowsWindow() = delete;

        WindowsWindow(const WindowsWindow&) = delete;
        WindowsWindow(const WindowsWindow&&) = delete;
        WindowsWindow & operator=(const WindowsWindow&) = delete;
        WindowsWindow & operator=(const WindowsWindow&&) = delete;

        ~WindowsWindow() override;

        void OnUpdate() override;

        [[nodiscard]] uint32_t GetWidth() const override;

        [[nodiscard]] uint32_t GetHeight() const override;

        void SetVSync(bool enable) override;

        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] void *GetNativeWindow() const override;

    private:
        void Init(const WindowProps & props);
        void ShutDown();

    private:
        GLFWwindow * m_Window {nullptr};
        OpenGLContext* m_Context {nullptr};

        struct WindowData
        {
            std::string m_Title;
            unsigned int m_Width, m_Height;
            bool m_VSync;
            // EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

} // OBase

#endif //OPENGL_WINDOWSWINDOW_H
