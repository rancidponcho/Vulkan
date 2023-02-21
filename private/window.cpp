#include "../public/window.hpp"

#include "../public/vkApplication.hpp"

void tk_window::init(const uint32_t width, const uint32_t height, vkApplication* app) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // GLFW originally meant for OpenGL

    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, app);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void tk_window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void tk_window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<vkApplication*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}