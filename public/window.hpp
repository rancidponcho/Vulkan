#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "global.hpp"

class tk_window {
   public:
    void init();
    void createSurface(VkInstance instance, VkSurfaceKHR surface);
    void destroy();
    void loop();

   private:
    GLFWwindow* window;

   public:
    GLFWwindow* get() { return window; }
};