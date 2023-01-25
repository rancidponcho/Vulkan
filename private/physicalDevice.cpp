#include "../public/physicalDevice.hpp"

// Physical Device // Considering ranking system for other devices
void tk_physicalDevice::select(VkInstance instance, VkSurfaceKHR surface, tk_swapChain swapChain) {
    uint32_t deviceCount{0};
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto &device : devices) {
        if (isSuitable(device, surface, swapChain)) {
            std::cout << "DONE!" << std::endl;
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find suitable GPU");
    }
}

bool tk_physicalDevice::isSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, tk_swapChain swapChain) {
    bool extensionsSupported = checkExtensionSupport(device);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = swapChain.querySupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return findQueueFamilies(surface).isComplete() && extensionsSupported && swapChainAdequate;
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

QueueFamilyIndices tk_physicalDevice::findQueueFamilies(VkSurfaceKHR surface) {
    // Setup vector of families available on device
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount{0};

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    // Find graphics & surface families.
    int i{0};
    for (const auto &queueFamily : queueFamilies) {
        // Could make these switch cases if requires more than one family
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        // Early exit incase family exists but is not VK_QUEUE_GRAPHICS_BIT.
        if (indices.isComplete()) {
            break;
        }
        i++;
    }
    
    std::cout << "e" << std::endl;
    return indices;
}