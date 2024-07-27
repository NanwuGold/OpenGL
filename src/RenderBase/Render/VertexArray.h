#ifndef HAZEL_VERTEXARRAY_H_
#define HAZEL_VERTEXARRAY_H_

#include <memory>
#include <vector>
#include "RenderBase/pointer_ptr.hpp"

namespace OBase
{

    class VertexBuffer;

    class IndexBuffer;
}

namespace OBase
{
    class VertexArray
    {
    public:
        VertexArray() = default;
        virtual ~VertexArray() = default;

        VertexArray(VertexArray&&) = delete;
        VertexArray(VertexArray&) = delete;

        VertexArray& operator=(VertexArray&&) = delete;
        VertexArray& operator=(VertexArray&) = delete;

        virtual void Bind() const = 0;

        [[maybe_unused]] virtual void UnBind() const = 0;

        [[maybe_unused]] virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;

        [[maybe_unused]] virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) = 0;

        [[maybe_unused]] [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const = 0;

        [[maybe_unused]] [[nodiscard]] virtual const Ref<IndexBuffer> &GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };

}

#endif // HAZEL_VERTEXARRAY_H_
