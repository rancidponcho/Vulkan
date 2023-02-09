#pragma once

#include <vulkan/vulkan.hpp>

class tk_instance;
class tk_window;

class tk_surface {
   public:
    void create(tk_instance &instance, tk_window &window);
    void destroy(tk_instance &instance);

   private:
    VkSurfaceKHR surface;

   public:
    VkSurfaceKHR get() { return surface; }
};