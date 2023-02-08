#include "../public/vkApplication.hpp"

void vkApplication::run() {
    window.init(WIDTH, HEIGHT);

    initVulkan();

    mainLoop();

    cleanup();
}

void vkApplication::initVulkan() {
    createInstance();
    debugMessenger.setup(instance);
    createSurface();
    physicalDevice.select(instance, surface, swapChain);
    device.create(physicalDevice, surface);
    swapChain.create(surface, physicalDevice, device, window);  // Includes viewImages creation, but should be seperated
    renderPass.create(swapChain, device);
    graphicsPipeline.create(device, renderPass);
    frameBuffer.create(swapChain, renderPass, device);
}

void vkApplication::mainLoop() {
    window.loop();
}

void vkApplication::cleanup() {
    frameBuffer.destroy(device);
    graphicsPipeline.destroy(device);
    renderPass.destroy(device);
    swapChain.destroy(device);
    device.destroy();
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

    // THIS GOES IN DEBUG MESSENGER CLASS (RENAME TO VALIDATION LAYERS)
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

void vkApplication::createSurface() {
    if (glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}
