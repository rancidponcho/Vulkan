#include "../public/frameBuffer.hpp"

void tk_frameBuffer::create(tk_swapChain swapChain, tk_renderPass renderPass, tk_logicalDevice device) {
    swapChainFramebuffers.resize(swapChain.getImageViews().size());

    for (size_t i = 0; i < swapChain.getImageViews().size(); i++) {
        VkImageView attachments[] = {swapChain.getImageViews()[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass.get();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.getExtent().width;
        framebufferInfo.height = swapChain.getExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device.get(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void tk_frameBuffer::destroy(tk_logicalDevice device) {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device.get(), framebuffer, nullptr);
    }
}