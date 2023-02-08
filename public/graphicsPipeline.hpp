#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

class tk_logicalDevice;
class tk_renderPass;

class tk_graphicsPipeline {
   public:
    void create(tk_logicalDevice &device, tk_renderPass &renderPass);
    void destroy(tk_logicalDevice &device);

   private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    // returns SPIR-V
    static std::vector<char> readFile(const std::string &filename);
    // VkShaderModule is a thin wrapper around the shader bytecode.
    // Conducts the compilation and linking of the SPIR-V to machine code for the GPU.
    VkShaderModule createShaderModule(const std::vector<char> &code, tk_logicalDevice &device);

   public:
};