#include "../public/vkApplication.hpp"

void vkApplication::run() {
    window.init(WIDTH, HEIGHT);
    initVulkan();
    mainLoop();
    cleanup();
}

void vkApplication::initVulkan() {
    instance.create(debugMessenger);
    debugMessenger.setup(instance);
    surface.create(instance, window);
    physicalDevice.select(instance, surface, swapChain);
    device.create(physicalDevice, surface);
    swapChain.create(surface, physicalDevice, device, window);  // Includes viewImages creation, but should be seperated
    renderPass.create(swapChain, device);
    graphicsPipeline.create(device, renderPass);
    frameBuffer.create(swapChain, renderPass, device);
    /* combine ? */
    commandPool.create(physicalDevice, surface, device);
    commandBuffer.create(commandPool, device);
}

void vkApplication::mainLoop() {
    window.loop();
}

void vkApplication::cleanup() {
    commandPool.destroy(device);
    frameBuffer.destroy(device);
    graphicsPipeline.destroy(device);
    renderPass.destroy(device);
    swapChain.destroy(device);
    device.destroy();
    debugMessenger.destroy(instance, nullptr);
    surface.destroy(instance);
    instance.destroy();
    window.destroy();
}