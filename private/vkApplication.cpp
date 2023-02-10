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
    createSyncObjects();
}

void vkApplication::mainLoop() {
    while (!glfwWindowShouldClose(window.get())) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device.get());
}

void vkApplication::cleanup() {
    vkDestroySemaphore(device.get(), imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device.get(), renderFinishedSemaphore, nullptr);
    vkDestroyFence(device.get(), inFlightFence, nullptr);

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

void vkApplication::drawFrame() {
    // Wait for previous frame to finish
    vkWaitForFences(device.get(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device.get(), 1, &inFlightFence);
    // Acquire an image from swap chain
    uint32_t imageIndex;
    vkAcquireNextImageKHR(device.get(), swapChain.get(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    // record a command buffer which draws the scene onto that image
    vkResetCommandBuffer(commandBuffer.get(), 0);
    commandBuffer.record(swapChain, renderPass, frameBuffer, graphicsPipeline, imageIndex);
    // Submit the recorded command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffer.getPtr();

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    // Present the swap chain image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain.get()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    // presentInfo.pResults = nullptr;

    vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);
}

void vkApplication::createSyncObjects() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(device.get(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
    }
}