#include "CEpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Confuse{
     static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type){
        switch(type){
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3: return GL_FLOAT;
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
        }

        CE_CORE_ASSERT(false, "unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray(){
        glCreateVertexArrays(1, &m_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray(){
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void OpenGLVertexArray::bind() const{
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::unbind() const{
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer){
        CE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "vertex buffer has no layout!");
        
        glBindVertexArray(m_rendererID);
        vertexBuffer->bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for(const auto& element: layout){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.getComponentCount(), shaderDataTypeToOpenGLBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)element.offset);
            index++;
        }
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer){
        glBindVertexArray(m_rendererID);
        indexBuffer->bind();

        m_indexBuffer = indexBuffer;
    }
}