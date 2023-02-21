#include "../public/commandBuffers.hpp"

#include "../public/commandPool.hpp"
#include "../public/frameBuffer.hpp"
#include "../public/geoBuffer.hpp"
#include "../public/geoData.hpp"
#include "../public/graphicsPipeline.hpp"
#include "../public/logicalDevice.hpp"
#include "../public/renderPass.hpp"
#include "../public/swapChain.hpp"

void tk_commandBuffers::create(tk_commandPool &commandPool, tk_logicalDevice &device) {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.get();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(device.get(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

// desperately needs work
void tk_commandBuffers::record(uint32_t currentFrame, tk_swapChain &swapChain, tk_renderPass &renderPass, tk_frameBuffer &frameBuffer, tk_graphicsPipeline &graphicsPipeline, uint32_t imageIndex, tk_geometryBuffer &geometryBuffer) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    // BEGIN CMD BUFFER
    if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    // RENDER PASS INFO STRUCT
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass.get();
    renderPassInfo.framebuffer = frameBuffer.get()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain.getExtent();
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    // BEGIN RENDER PASS
    vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // BIND GRAPHICS PIPELINE
    vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.get());

    // BASIC DRAWING COMMANDS
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain.getExtent().width);
    viewport.height = static_cast<float>(swapChain.getExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChain.getExtent();
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

    // VERTEX BUFFERS
    VkBuffer vertexBuffers[] = {geometryBuffer.get()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
    size_t vertexDataSize = sizeof(vertices[0]) * vertices.size();
    vkCmdBindIndexBuffer(commandBuffers[currentFrame], geometryBuffer.get(), vertexDataSize, VK_INDEX_TYPE_UINT16);

    // DRAW
    vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    // END RENDER PASS
    vkCmdEndRenderPass(commandBuffers[currentFrame]);

    // END CMD BUFFER
    if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}