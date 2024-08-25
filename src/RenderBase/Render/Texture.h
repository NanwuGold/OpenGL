#ifndef OPENGL_RENDERBASE_TEXTURE_H
#define OPENGL_RENDERBASE_TEXTURE_H

#include "RenderBase/pointer_ptr.hpp"
#include <glad/glad.h>


enum class MultiSample : uint8_t
{
    None,
    X4 = 4,
    X6 = 6,
    X8 = 8,
    X16 = 16,
    X32 = 32,
};

class Texture
{
public:

    Texture() = default;

    Texture(Texture&&) = delete;
    Texture(Texture&) = delete;

    Texture& operator=(Texture&&) = delete;
    Texture& operator=(Texture&) = delete;

    virtual ~Texture() = default;

    /**
     * @brief 绑定纹理
     */
    virtual void Bind() = 0;

    /**
     * @brief 解绑纹理
     */
    virtual void UnBind() = 0;

    /**
     * @brief 纹理作为图片绑定到绑定点
     * @param bindingPoint uint8_t
     */
    virtual void BindImage(uint8_t bindingPoint) = 0;

    /**
     * @brief 取消纹理作为图片绑定到绑定点
     */
    virtual void UnBindImage() = 0;

    /**
     * @brief 重新设置纹理大小
     * @param w int 宽
     * @param h int  高
     */
    virtual void resize(int w,int h) = 0;

    /**
     * @brief 创建纹理
     */
    virtual void Create() = 0;

    /**
     * @brief 获取纹理宽度
     * @return int
     */
    virtual unsigned int GetWidth() = 0;

    /**
     * @brief 获取纹理高度
     * @return int
     */
    virtual unsigned int GetHeight() = 0;

    /**
     * @brief 获取纹理数据在显存中的存储格式
     * @return GLenum
     */
    virtual GLenum InternalFormat() const = 0;

    /**
     * @brief 纹理ID
     * @return unsigned int
     */
    virtual unsigned int RenderID() = 0;

    /**
     * @brief 是否支持多采样
     * @return bool
     */
    virtual bool multiSampleFlag() = 0;

    /**
     * @brief 使用数据清空纹理
     * @param level GLuint 纹理MipMap级别
     * @param data void* 清空数据
     */
    virtual void Clear(GLint level = 0, const void* data = nullptr) = 0;

    static OBase::Ref<Texture> Create(int w, int h, GLenum format, MultiSample sample = MultiSample::None);
};


#endif //OPENGL_RENDERBASE_TEXTURE_H
