#include "../public/window.hpp"

void tk_window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // GLFW originally meant for OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);    // window resizing requires extra care

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void tk_window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// temporary main loop function
void tk_window::loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}