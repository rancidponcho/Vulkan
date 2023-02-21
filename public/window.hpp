#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>

class vkApplication;

class tk_window {
   public:
    void init(const uint32_t width, const uint32_t height, vkApplication* app);
    void destroy();

   private:
    GLFWwindow* window;
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

   public:
    GLFWwindow* get() { return window; }
};