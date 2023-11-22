#include "core/ce_Device.hpp"

namespace ConfuseEngine {
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
      if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
      } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
      }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
      auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
      if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
      }
    }

    CE_Device::CE_Device(CE_Window &window) : m_rWindow{window} {
      createInstance();
      setupDebugMessenger();
      createSurface();
      pickPhysicalDevice();
      createLogicalDevice();
      createCommandPool();
    }

    CE_Device::~CE_Device() {
      vkDestroyCommandPool(m_device, m_commandPool, nullptr);
      vkDestroyDevice(m_device, nullptr);

      if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
      }

      vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
      vkDestroyInstance(m_instance, nullptr);
    }

    void CE_Device::createInstance() {
      if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo = {};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "Confuse Editor";
      appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 2);
      appInfo.pEngineName = "ConfuseEngine";
      appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 2);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      auto extensions = getRequiredExtensions();
      createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
      if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
      } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
      }

      hasGflwRequiredInstanceExtensions();
    }

    void CE_Device::pickPhysicalDevice() {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
      if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
      }
      std::cout << "Device count: " << deviceCount << std::endl;
      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

      for (const auto &device : devices) {
        if (isPreferredDevice(device)) {
          m_physicalDevice = device;
          return;
        }
      }

      for(const auto& device: devices){
        if(isSuitableDevice(device)){
          m_physicalDevice = device;
          break;
        }
      }

      if (m_physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
      }

      vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);
      std::cout << "physical device: " << properties.deviceName << std::endl;
    }

    void CE_Device::createLogicalDevice() {
      QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

      float queuePriority = 1.0f;
      for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures = {};
      deviceFeatures.samplerAnisotropy = VK_TRUE;

      VkDeviceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos = queueCreateInfos.data();

      createInfo.pEnabledFeatures = &deviceFeatures;
      createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

      if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
      } else {
        createInfo.enabledLayerCount = 0;
      }

      if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
      }

      vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
      vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);
    }

    void CE_Device::createCommandPool() {
      QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

      VkCommandPoolCreateInfo poolInfo = {};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
      poolInfo.flags =
          VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
      }
    }

    void CE_Device::createSurface() { m_rWindow.createWindowSurface(m_instance, &m_surface); }


    bool CE_Device::isPreferredDevice(VkPhysicalDevice device) {
      if(!isSuitableDevice(device))
        return false;

      auto props = VkPhysicalDeviceProperties{};
      vkGetPhysicalDeviceProperties(device, &props);

      return props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    }

    bool CE_Device::isSuitableDevice(VkPhysicalDevice device){
      QueueFamilyIndices indices = findQueueFamilies(device);

      bool extensionsSupported = checkDeviceExtensionSupport(device);

      bool swapChainAdequate = false;
      if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
      }

      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

      return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    void CE_Device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
      createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugCallback;
      createInfo.pUserData = nullptr;
    }

    void CE_Device::setupDebugMessenger() {
      if (!enableValidationLayers) return;
      VkDebugUtilsMessengerCreateInfoEXT createInfo;
      populateDebugMessengerCreateInfo(createInfo);
      if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
      }
    }

    bool CE_Device::checkValidationLayerSupport() {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const char *layerName : m_validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers) {
          if (strcmp(layerName, layerProperties.layerName) == 0) {
            layerFound = true;
            break;
          }
        }

        if (!layerFound) {
          return false;
        }
      }

      return true;
    }

    std::vector<const char *> CE_Device::getRequiredExtensions() {
      uint32_t glfwExtensionCount = 0;
      const char **glfwExtensions;
      glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

      if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      return extensions;
    }

    void CE_Device::hasGflwRequiredInstanceExtensions() {
      uint32_t extensionCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
      std::vector<VkExtensionProperties> extensions(extensionCount);
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

      std::cout << "available extensions:" << std::endl;
      std::unordered_set<std::string> available;
      for (const auto &extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
        available.insert(extension.extensionName);
      }

      std::cout << "required extensions:" << std::endl;
      auto requiredExtensions = getRequiredExtensions();
      for (const auto &required : requiredExtensions) {
        std::cout << "\t" << required << std::endl;
        if (available.find(required) == available.end()) {
          throw std::runtime_error("Missing required glfw extension");
        }
      }
    }

    bool CE_Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(
          device,
          nullptr,
          &extensionCount,
          availableExtensions.data());

      std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

      for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
    }

    QueueFamilyIndices CE_Device::findQueueFamilies(VkPhysicalDevice device) {
      QueueFamilyIndices indices;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          indices.graphicsFamily = i;
          indices.graphicsFamilyHasValue = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
        if (queueFamily.queueCount > 0 && presentSupport) {
          indices.presentFamily = i;
          indices.presentFamilyHasValue = true;
        }
        if (indices.isComplete()) {
          break;
        }

        i++;
      }

      return indices;
    }

    SwapChainSupportDetails CE_Device::querySwapChainSupport(VkPhysicalDevice device) {
      SwapChainSupportDetails details;
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

      uint32_t formatCount;
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

      if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
      }

      uint32_t presentModeCount;
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

      if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
      }
      return details;
    }

    VkFormat CE_Device::findSupportedFormat(
        const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
      for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
          return format;
        } else if (
            tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
          return format;
        }
      }
      throw std::runtime_error("failed to find supported format!");
    }

    uint32_t CE_Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);
      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
          return i;
        }
      }

      throw std::runtime_error("failed to find suitable memory type!");
    }

    void CE_Device::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(m_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
      }

      vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
    }

    VkCommandBuffer CE_Device::beginSingleTimeCommands() {
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = m_commandPool;
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer commandBuffer;
      vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      vkBeginCommandBuffer(commandBuffer, &beginInfo);
      return commandBuffer;
    }

    void CE_Device::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
      vkEndCommandBuffer(commandBuffer);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(m_graphicsQueue);

      vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
    }

    void CE_Device::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkBufferCopy copyRegion{};
      copyRegion.srcOffset = 0;
      copyRegion.dstOffset = 0;
      copyRegion.size = size;
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(commandBuffer);
    }

    void CE_Device::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkBufferImageCopy region{};
      region.bufferOffset = 0;
      region.bufferRowLength = 0;
      region.bufferImageHeight = 0;

      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = layerCount;

      region.imageOffset = {0, 0, 0};
      region.imageExtent = {width, height, 1};

      vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
      endSingleTimeCommands(commandBuffer);
    }

    void CE_Device::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) {
      if (vkCreateImage(m_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
      }

      VkMemoryRequirements memRequirements;
      vkGetImageMemoryRequirements(m_device, image, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
      }

      if (vkBindImageMemory(m_device, image, imageMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind image memory!");
      }
    }

    void CE_Device::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, uint32_t layerCount) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkImageMemoryBarrier barrier{};
      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barrier.oldLayout = oldLayout;
      barrier.newLayout = newLayout;

      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

      barrier.image = image;
      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = mipLevels;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = layerCount;

      if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
          barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
      } else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      }

      VkPipelineStageFlags sourceStage;
      VkPipelineStageFlags destinationStage;

      if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      } else if (
          oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      } else if (
          oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
          newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      } else if (
          oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
          newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask =
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
      } else {
        throw std::invalid_argument("unsupported layout transition!");
      }
      vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
      endSingleTimeCommands(commandBuffer);
    }

}