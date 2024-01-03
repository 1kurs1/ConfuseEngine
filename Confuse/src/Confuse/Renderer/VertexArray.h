#pragma once

#include <memory>
#include "Confuse/Renderer/Buffer.h"

namespace Confuse{
    class VertexArray{
    public:
        virtual ~VertexArray(){}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void setIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffers() const = 0;

        static VertexArray* create();
    };
}