#include "../public/vkApplication.hpp"

void vkApplication::run() {
    window.init(WIDTH, HEIGHT, this);
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

    commandPool.create(physicalDevice, surface, device);
    commandBuffers.create(commandPool, device);
    vertexBuffer.create(device, physicalDevice, commandPool);

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
    for (size_t i{0}; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device.get(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device.get(), renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device.get(), inFlightFences[i], nullptr);
    }

    commandPool.destroy(device);
    vertexBuffer.destroy(device);
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
    vkWaitForFences(device.get(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // Determine if swapChain must be recreated -----------------------------
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device.get(), swapChain.get(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(device.get(), 1, &inFlightFences[currentFrame]);

    // record a command buffer which draws the scene onto that image
    vkResetCommandBuffer(commandBuffers.get(currentFrame), 0);
    commandBuffers.record(currentFrame, swapChain, renderPass, frameBuffer, graphicsPipeline, imageIndex, vertexBuffer);

    // Submit the recorded command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffers.getPtr(currentFrame);

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
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

    result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);
    // --------------------------------------
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }

    // iterate current frame
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void vkApplication::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for (size_t i{0}; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device.get(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects!");
        }
    }
}

// -----------------------------------------
void vkApplication::recreateSwapChain() {
    int width{0}, height{0};
    glfwGetFramebufferSize(window.get(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window.get(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device.get());

    // cleanup
    for (size_t i{0}; i < frameBuffer.get().size(); i++) {
        vkDestroyFramebuffer(device.get(), frameBuffer.get()[i], nullptr);
    }
    for (size_t i{0}; i < swapChain.getImageViews().size(); i++) {
        vkDestroyImageView(device.get(), swapChain.getImageViews()[i], nullptr);
    }
    vkDestroySwapchainKHR(device.get(), swapChain.get(), nullptr);

    swapChain.create(surface, physicalDevice, device, window);
    frameBuffer.create(swapChain, renderPass, device);
}