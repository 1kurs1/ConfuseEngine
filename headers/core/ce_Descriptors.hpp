#pragma once

#include "core/ce_Device.hpp"

namespace ConfuseEngine {
    class CE_DescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(CE_Device &device) : m_Device{device} {}

            Builder &addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
            std::unique_ptr<CE_DescriptorSetLayout> build() const;

        private:
            CE_Device &m_Device;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        CE_DescriptorSetLayout(CE_Device &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~CE_DescriptorSetLayout();
        CE_DescriptorSetLayout(const CE_DescriptorSetLayout&) = delete;
        CE_DescriptorSetLayout &operator=(const CE_DescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }

    private:
        CE_Device &m_rDevice;
        VkDescriptorSetLayout m_descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

        friend class CE_DescriptorWriter;
    };

    class CE_DescriptorPool {
    public:
        class Builder {
        public:
            Builder(CE_Device &device) : m_Device{device} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<CE_DescriptorPool> build() const;

        private:
            CE_Device &m_Device;
            std::vector<VkDescriptorPoolSize> m_poolSizes{};
            uint32_t m_maxSets = 1000;
            VkDescriptorPoolCreateFlags m_poolFlags = 0;
        };

        CE_DescriptorPool(CE_Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~CE_DescriptorPool();
        CE_DescriptorPool(const CE_DescriptorPool&) = delete;
        CE_DescriptorPool &operator=(const CE_DescriptorPool&) = delete;

        bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
        void resetPool();

        VkDescriptorPool& getDescriptorPool(){return m_descriptorPool;}

    private:
        CE_Device &m_rDevice;
        VkDescriptorPool m_descriptorPool;

        friend class CE_DescriptorWriter;
    };

    class CE_DescriptorWriter {
    public:
        CE_DescriptorWriter(CE_DescriptorSetLayout &setLayout, CE_DescriptorPool &pool);

        CE_DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        CE_DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        CE_DescriptorSetLayout &m_rSetLayout;
        CE_DescriptorPool &m_rPool;
        std::vector<VkWriteDescriptorSet> m_writes;
    };
}