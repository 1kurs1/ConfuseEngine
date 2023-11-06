#include "core/ce_Descriptors.hpp"
 
namespace ConfuseEngine { 
    CE_DescriptorSetLayout::Builder &CE_DescriptorSetLayout::Builder::addBinding(uint32_t binding,VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count) {
        assert(m_bindings.count(binding) == 0 && "binding already in use");
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = descriptorType;
        layoutBinding.descriptorCount = count;
        layoutBinding.stageFlags = stageFlags;
        m_bindings[binding] = layoutBinding;
        return *this;
    }
    
    std::unique_ptr<CE_DescriptorSetLayout> CE_DescriptorSetLayout::Builder::build() const {
        return std::make_unique<CE_DescriptorSetLayout>(m_rDevice, m_bindings);
    }

    CE_DescriptorSetLayout::CE_DescriptorSetLayout(CE_Device &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) : m_rDevice{device}, m_bindings{bindings} {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
        for (auto kv : bindings) {
            setLayoutBindings.push_back(kv.second);
        }
        
        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
        descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();
        
        if (vkCreateDescriptorSetLayout(m_rDevice.device(), &descriptorSetLayoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }
    
    CE_DescriptorSetLayout::~CE_DescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(m_rDevice.device(), m_descriptorSetLayout, nullptr);
    }
    
    CE_DescriptorPool::Builder &CE_DescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count) {
        m_poolSizes.push_back({descriptorType, count});
        return *this;
    }
    
    CE_DescriptorPool::Builder &CE_DescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags) {
        m_poolFlags = flags;
        return *this;
    }

    CE_DescriptorPool::Builder &CE_DescriptorPool::Builder::setMaxSets(uint32_t count) {
        m_maxSets = count;
        return *this;
    }
    
    std::unique_ptr<CE_DescriptorPool> CE_DescriptorPool::Builder::build() const {
        return std::make_unique<CE_DescriptorPool>(m_rDevice, m_maxSets, m_poolFlags, m_poolSizes);
    }
    
    CE_DescriptorPool::CE_DescriptorPool(CE_Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes) : m_rDevice{device} {
        VkDescriptorPoolCreateInfo descriptorPoolInfo{};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        descriptorPoolInfo.pPoolSizes = poolSizes.data();
        descriptorPoolInfo.maxSets = maxSets;
        descriptorPoolInfo.flags = poolFlags;
        
        if (vkCreateDescriptorPool(m_rDevice.device(), &descriptorPoolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
    
    CE_DescriptorPool::~CE_DescriptorPool() {
        vkDestroyDescriptorPool(m_rDevice.device(), m_descriptorPool, nullptr);
    }
    
    bool CE_DescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        allocInfo.descriptorSetCount = 1;

        if (vkAllocateDescriptorSets(m_rDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
            return false;
        }
        return true;
    }
    
    void CE_DescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const {
        vkFreeDescriptorSets(m_rDevice.device(), m_descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
    }
    
    void CE_DescriptorPool::resetPool() {
        vkResetDescriptorPool(m_rDevice.device(), m_descriptorPool, 0);
    }
    
    CE_DescriptorWriter::CE_DescriptorWriter(CE_DescriptorSetLayout &setLayout, CE_DescriptorPool &pool) : m_rSetLayout{setLayout}, pool{pool} {}
    
    CE_DescriptorWriter &CE_DescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) {
        assert(m_rSetLayout.m_bindings.count(binding) == 1 && "layout does not contain specified binding");
        
        auto &bindingDescription = m_rSetLayout.m_bindings[binding];
        
        assert(bindingDescription.descriptorCount == 1 && "binding single descriptor info, but binding expects multiple");
        
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pBufferInfo = bufferInfo;
        write.descriptorCount = 1;
        
        writes.push_back(write);
        return *this;
    }
    
    CE_DescriptorWriter &CE_DescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) {
        assert(m_rSetLayout.m_bindings.count(binding) == 1 && "layout does not contain specified binding");
        
        auto &bindingDescription = m_rSetLayout.m_bindings[binding];
        
        assert(bindingDescription.descriptorCount == 1 && "binding single descriptor info, but binding expects multiple");
        
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pImageInfo = imageInfo;
        write.descriptorCount = 1;
        
        writes.push_back(write);
        return *this;
    }
    
    bool CE_DescriptorWriter::build(VkDescriptorSet &set) {
        bool success = pool.allocateDescriptor(m_rSetLayout.getDescriptorSetLayout(), set);
        if (!success) {
            return false;
        }
        overwrite(set);
        return true;
    }
    
    void CE_DescriptorWriter::overwrite(VkDescriptorSet &set) {
        for (auto &write : writes) {
            write.dstSet = set;
        }
        vkUpdateDescriptorSets(pool.m_rDevice.device(), writes.size(), writes.data(), 0, nullptr);
    }
}