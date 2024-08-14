#ifndef OPENGL_RENDERBASE_SHADERSTORAGEBUFFEROOBJECT_H
#define OPENGL_RENDERBASE_SHADERSTORAGEBUFFEROOBJECT_H

#include <RenderBase/pointer_ptr.hpp>

namespace OBase
{

class ShaderStorageBuffer
{
public:
    virtual ~ShaderStorageBuffer() = default;
    ShaderStorageBuffer() = default;

    ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
    ShaderStorageBuffer(ShaderStorageBuffer&&) = delete;

    ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
    ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) = delete;

    /**
     * @brief 绑定缓冲对象到缓冲区
     */
    virtual void Bind() const = 0;

    /**
     * @brief 取消绑定当前对象
     */
    virtual void UnBind()= 0;

    /**
     * @brief 获取当前的缓冲对象
     * @return unsigned int 当前的缓冲对象
     */
    virtual unsigned int RenderID()= 0;

    /**
     * @brief 绑定Shader Storage Buffer 对象到绑定点
     * @param bindPoint  绑定点
     */
    virtual void LinkBindingPoint(uint8_t bindPoint) = 0;

    /**
     * @brief 获取设置的绑定点
     * @return uint8_t 当前绑定的绑定点
     */
    virtual uint8_t BindPoint() const = 0;

    /**
     * @brief 修改 Shader Storage Buffer 对象的数据
     * @param offset uint32_t 偏移位置
     * @param size uint32_t 设置的数据的大小
     * @param data void * 数据的指针
     */
    virtual void UpdateData(uint32_t offset, uint32_t size, const void* data) = 0;

    /**
     * @brief 返回一个ShaderStorageBuffer 对象
     * @param size ShaderStorageBuffer对象的内存的大小
     * @return Ref<ShaderStorageBuffer>
     */
    static Ref<ShaderStorageBuffer> Create(uint32_t size);
};


class OpenGLShaderStorageBuffer final: public  ShaderStorageBuffer
{
public:
    explicit OpenGLShaderStorageBuffer(uint32_t size);
    ~OpenGLShaderStorageBuffer() override;

    OpenGLShaderStorageBuffer(const OpenGLShaderStorageBuffer&) = delete;
    OpenGLShaderStorageBuffer(OpenGLShaderStorageBuffer&&) = delete;

    OpenGLShaderStorageBuffer& operator=(const OpenGLShaderStorageBuffer&) = delete;
    OpenGLShaderStorageBuffer& operator=(OpenGLShaderStorageBuffer&&) = delete;

    void Bind() const override;
    void UnBind() override;
    void LinkBindingPoint(uint8_t bindPoint) override;

    [[nodiscard]] uint8_t BindPoint() const override;
    [[nodiscard]] unsigned int RenderID() override;

    void UpdateData(uint32_t offset, uint32_t size, const void* data) override;

private:
    void Invalidate();

private:
    uint32_t m_Size;           ///< buffer size
    unsigned int m_RenderID;   ///< Render ID
    uint8_t m_BindPoint;       ///< Bind Point

};

}



#endif //!OPENGL_RENDERBASE_SHADERSTORAGEBUFFEROOBJECT_H