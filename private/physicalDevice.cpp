#include "../public/physicalDevice.hpp"

#include <set>
#include <stdexcept>
#include <vector>

#include "../public/instance.hpp"
#include "../public/surface.hpp"
#include "../public/swapChain.hpp"

// Physical Device // Considering ranking system for other devices
void tk_physicalDevice::select(tk_instance &instance, tk_surface &surface, tk_swapChain &swapChain) {
    uint32_t deviceCount{0};
    vkEnumeratePhysicalDevices(instance.get(), &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.get(), &deviceCount, devices.data());

    for (const auto &device : devices) {
        if (isSuitable(device, surface.get(), swapChain)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find suitable GPU");
    }
}

bool tk_physicalDevice::isSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, tk_swapChain &swapChain) {
    bool extensionsSupported = checkExtensionSupport(device);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = swapChain.querySupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return findQueueFamilies(device, surface).isComplete() && extensionsSupported && swapChainAdequate;
}

bool tk_physicalDevice::checkExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices tk_physicalDevice::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount{0};

    /* SEGFAULT HERE */
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Find graphics & surface families.
    int i{0};
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        // Early exit incase family exists but is not VK_QUEUE_GRAPHICS_BIT.
        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

QueueFamilyIndices tk_physicalDevice::findQueueFamilies(tk_surface &surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount{0};

    /* SEGFAULT HERE */
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    // Find graphics & surface families.
    int i{0};
    for (const auto &queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface.get(), &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        // Early exit incase family exists but is not VK_QUEUE_GRAPHICS_BIT.
        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

uint32_t tk_physicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}