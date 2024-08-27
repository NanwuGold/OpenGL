#ifndef OBASE_WINDOW_H
#define OBASE_WINDOW_H

#include <functional>
#include <string>

#include <RenderBase/Event/Event.h>

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
        using EventCallbackFn = std::function<void(Event &)>;
    public:
        Window() = default;
        virtual ~Window() = default;

        Window(const Window& other) = delete;
        Window(Window&& other) noexcept = delete;
        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = delete;

        /**
         * @brief update
         */
        virtual void OnUpdate() = 0;

        /**
         * @brief 获取窗口的宽
         * @return uint32_t
         */
        [[nodiscard]] virtual uint32_t GetWidth() const = 0;

        /**
         * @brief 获取窗口的高
         * @return uint32_t
         */
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        /**
         * @brief 设置事件回调
         * @param callback 回调函数
         */
        virtual void SetEventCallBack(const EventCallbackFn &callback) = 0;

        /**
         * @brief 垂直同步
         * @param enable bool
         */
        virtual void SetVSync(bool enable) = 0;

        /**
         * @brief 获取垂直同步状态
         * @return bool
         */
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