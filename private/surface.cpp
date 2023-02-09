#include "../public/surface.hpp"

#include <GLFW/glfw3.h>

#include "../public/instance.hpp"
#include "../public/window.hpp"

void tk_surface::create(tk_instance &instance, tk_window &window) {
        if (glfwCreateWindowSurface(instance.get(), window.get(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void tk_surface::destroy(tk_instance &instance) {
    vkDestroySurfaceKHR(instance.get(), surface, nullptr);
} 