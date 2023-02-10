#include "../public/window.hpp"

void tk_window::init(const uint32_t width, const uint32_t height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // GLFW originally meant for OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);    // window resizing requires extra care

    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

void tk_window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}