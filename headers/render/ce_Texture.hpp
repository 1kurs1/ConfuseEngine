#pragma once 

#include "core/ce_Device.hpp"

using namespace ConfuseEngine;
namespace ConfuseEngineRenderer{
    class CE_Texture{
    public:
        CE_Texture(CE_Device& device, const std::string& textureFilePath);
        CE_Texture(CE_Device& device, VkFormat format, VkExtent3D extent, VkImageUsageFlags usage, VkSampleCountFlagBits sampleCount);
        ~CE_Texture();

        CE_Texture(const CE_Texture&) = delete;
        CE_Texture& operator=(const CE_Texture&) = delete;

        VkImageView imageView() const {return m_textureImageView;}
        VkSampler sampler() const {return m_textureSampler;}
        VkImage getImage() const {return m_textureImage;}
        VkImageView getImageView() const {return m_textureImageView;}
        VkDescriptorImageInfo getImageInfo() const {return m_descriptor;}
        VkImageLayout getImageLayout() const {return m_textureLayout;}
        VkExtent3D getExtent() const {return m_extent;}
        VkFormat getFormat() const {return m_format;}

        void updateDescriptor();
        void transitionLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);

        static std::unique_ptr<CE_Texture> createTextureFromFile(CE_Device& device, const std::string& filePath);

        private:
            void createTextureImage(const std::string& filePath);
            void createTextureImageView(VkImageViewType viewType);
            void createTextureSampler();

            VkDescriptorImageInfo m_descriptor{};

            CE_Device& m_rDevice;
            VkImage m_textureImage = nullptr;
            VkDeviceMemory m_textureImageMemory = nullptr;
            VkImageView m_textureImageView = nullptr;
            VkSampler m_textureSampler = nullptr;
            VkFormat m_format;
            VkImageLayout m_textureLayout;
            uint32_t m_mipLevels{1};
            uint32_t m_layerCount{1};
            VkExtent3D m_extent{};
    };
}