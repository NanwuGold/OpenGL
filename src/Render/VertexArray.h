#ifndef HAZEL_VERTEXARRAY_H_
#define HAZEL_VERTEXARRAY_H_

#include <memory>
#include <vector>
#include "pointer_ptr.hpp"

namespace FXAA
{

    class VertexBuffer;

    class IndexBuffer;
}

namespace FXAA
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

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
