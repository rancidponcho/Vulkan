#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

#include "public/vkApplication.hpp"

int main() {
    vkApplication app;

    try {
        app.run();  // constructor/destructor vs run()/cleanup() ?
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}