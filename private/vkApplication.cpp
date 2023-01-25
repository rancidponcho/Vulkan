#include "../public/vkApplication.hpp"

void vkApplication::run() {
    window.init();

    initVulkan();

    mainLoop();

    cleanup();
}

void vkApplication::initVulkan() {
    createInstance();
    debugMessenger.setup(instance);
    createSurface();

    physicalDevice.select(instance, surface, swapChain);

    createLogicalDevice();
    swapChain.create(physicalDevice.get(), surface, physicalDevice.findQueueFamilies(surface), window.get(), device);
}

void vkApplication::mainLoop() {
    window.loop();
}

void vkApplication::cleanup() {
    swapChain.destroy(device);

    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
        debugMessenger.destroy(instance, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    window.destroy();
}

void vkApplication::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "vkApplication";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Enable extensions
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Enable validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        // enable debug messenger
        debugMessenger.populateCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;  // ?
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    //	instance now can be created.
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }
}

// Extensions & Validation Layers
bool vkApplication::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    std::set<std::string> requiredLayers(validationLayers.begin(), validationLayers.end());
    for (const auto &layer : availableLayers) {
        requiredLayers.erase(layer.layerName);
    }

    return requiredLayers.empty();
}

std::vector<const char *> vkApplication::getRequiredExtensions() {
    uint32_t glfwExtensionCount{};
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);  // appends debug utilities extension to list of required extensions
    }

    return extensions;
}

// Logical Device
void vkApplication::createLogicalDevice() {
    // Specify queues to be created
    QueueFamilyIndices indices = physicalDevice.findQueueFamilies(surface);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;  // Inflences sheduling of command buffers
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    // Create logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    // Retrieve queue handle
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void vkApplication::createSurface() {
    if (glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

// ================================================================================================================= //
// Hey you go reread the "window" chapter. You don't understand logical device.
