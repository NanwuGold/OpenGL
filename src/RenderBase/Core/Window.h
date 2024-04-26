#ifndef OBASE_WINDOW_H
#define OBASE_WINDOW_H

#include <functional>
#include <string>

namespace OBase
{
    struct WindowProps
    {
        std::string m_Title;
        int m_W = 800;
        int m_H = 600;

        WindowProps()
        :WindowProps(800,600,"Render Window")
        {

        }

        explicit WindowProps(const std::string & title)
                :WindowProps(800,600,title)
        {

        }

        WindowProps(const int w, const int h, std::string wt)
                : m_Title(std::move(wt)), m_W(w), m_H(h)
            {

        }

        [[nodiscard]] int width() const { return m_W; }
        [[nodiscard]] int height() const { return m_H; }
        [[nodiscard]] const std::string & title() const { return m_Title; }
    };

    class Window
    {
    public:
        Window() = default;

        Window(const Window& other) = delete;
        Window(Window&& other) noexcept = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = delete;

        virtual ~Window() = default;

        // using EventCallbackFn = std::function<void(Event &)>;

        virtual void OnUpdate() = 0;

        [[nodiscard]] virtual uint32_t GetWidth() const = 0;

        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        // virtual void SetEventCallBack(const EventCallbackFn &callback) = 0;
        virtual void SetVSync(bool enable) = 0;

        [[maybe_unused]] [[nodiscard]] virtual bool IsVSync() const = 0;

        /**
         * @brief 获取当前的窗口指针
         * @return void* 当前窗口
         */
        [[nodiscard]] virtual void *GetNativeWindow() const = 0;

        /**
         * @brief 创建一个窗口
         * @param props 指定窗口的信息
         * @return 创建完成的窗口
         */
        static Window *Create(const WindowProps &props = WindowProps());
    };

}

#endif