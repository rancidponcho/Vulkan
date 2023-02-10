#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>

class tk_window {
   public:
    void init(const uint32_t width, const uint32_t height);
    void destroy();

   private:
    GLFWwindow* window;

   public:
    GLFWwindow* get() { return window; }
};