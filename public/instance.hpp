#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

class tk_debugMessenger;

class tk_instance {
   public:
    void create(tk_debugMessenger &debugMessenger);
    void destroy();

   private:
    VkInstance instance;
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions();

   public:
    VkInstance get() { return instance; }
};