#pragma once

#include "core/ce_Device.hpp"
#include "core/ce_Buffer.hpp"

namespace ConfuseEngineRenderer{
    class CE_Model{
    public:

        struct Vertex{
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Builder{
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string& filePath);
        };

        CE_Model(ConfuseEngine::CE_Device& device, const CE_Model::Builder& builder);
        ~CE_Model();

        CE_Model(const CE_Model&) = delete;
        CE_Model& operator=(const CE_Model&) = delete;

        static std::unique_ptr<CE_Model> createModelFromFile(ConfuseEngine::CE_Device& device, const std::string& filePath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex>& vertices);
        void createIndexBuffers(const std::vector<uint32_t>& indices);

        ConfuseEngine::CE_Device& m_rDevice;

        std::unique_ptr<ConfuseEngine::CE_Buffer> m_pVertexBuffer;
        uint32_t m_vertexCount;

        bool m_hasIndexBuffer = false;

        std::unique_ptr<ConfuseEngine::CE_Buffer> m_pIndexBuffer;
        uint32_t m_indexCount;
    };
}