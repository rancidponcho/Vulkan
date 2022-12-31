#include <vulkan/vulkan.h>		// LunarG SDK

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>			// Cross-platform window for graphics libraries (GL)

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;		// actual window of app
	VkInstance instance;	// connection between app & the Vk library
	
	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// GLFW originally meant for OpenGL
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);		// window resizing requires extra care
		
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	
	}	

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);	// optional allocator callback ignored by passing nullptr

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void createInstance() {
		/*
			The general pattern that object creation function parameters in follow is:
				1) Pointer to struct with creation info
				2) Pointer to custom allocator callbacks (ignored by passing nullptr to vkDestroyInstance())
				3) Pointer to the variable that stores the handle to the new object
		*/
		//	1) OPTIONAL
		//	Provides Vk driver with application information for optimization on different devices.
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		
		//	2) OPTIONAL
		//	Using nullptr for allocator callback

		//	3) REQUIRED
		//	Tells Vk driver which global extensions and validation layers to use. 
		//	"Global" means it applies to the ENTIRE program & not a specific device.
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		//	Vk is platform agnostic -> we need an extension to interface with the window system.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		//	We must find required extension(s) of windowing application (GLFW)	
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		//	Global validation layers must be determined and enabled
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		//	Now that parameters are specified, a Vk instance can be created & checked for success
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}	
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
