#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>

#include <vulkan/vulkan.h> // LunarG SDK

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> // Cross-platform window for graphics libraries (GL)

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// validation layers
const std::vector<const char *> validationLayers = {
	"VK_LAYER_KHRONOS_validation" // standard validation is bundled in this specification
};

#ifndef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication
{
public:
	/* */
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow *window;	 // actual window of app
	VkInstance instance; // connection between app & the Vk library

	// INITIALIZATION //
	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // GLFW originally meant for OpenGL
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);	  // window resizing requires extra care

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void initVulkan()
	{
		createInstance();
	}

	void createInstance()
	{
		// supportedExtensions();

		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not available!");
		}

		/*
			The general pattern that object creation function parameters in follow is:
				1) Pointer to struct with creation info
				2) Pointer to custom allocator callbacks
				3) Pointer to the variable that stores the handle to the new object
		*/

		//	1)	Provides Vk driver with application information for optimization on different devices.
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		//	2)	Using nullptr for allocator callback

		//	3)	Tells Vk driver which global extensions and validation layers to use.
		//	    "Global" means it applies to the ENTIRE program & not a specific device.
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		//	Vk is platform agnostic -> we need an extension to interface with the window system.
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); //	find required extension(s) of windowing application (GLFW)

		//	Global validation layers must be determined and enabled
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		//	Now that parameters are specified, a Vk instance can be created & checked for success
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}
	}

	/* ========== UTILITIES ========== */
	// lists supported extensions and those that are glfw-required
	void supportedExtensions()
	{
		// available VK extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		std::cout << "availiable extensions:\n";

		// glfw-required extensions
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		if (glfwExtensionCount == 0)
		{
			throw std::runtime_error("Failed to find GLFW extensions!");
		}

		// lists to terminal
		for (const auto &extension : extensions)
		{
			std::cout << '\t' << extension.extensionName;

			// check if required for glfw
			for (int i = 0; i < glfwExtensionCount; i++)
			{
				std::string extensionName = glfwExtensions[i];
				if (extension.extensionName == extensionName)
				{
					std::cout << " -- GLFW-REQUIRED";
				}
			}
			std::cout << '\n';
		}
	}

	bool checkValidationLayerSupport()
	{
		// available validation layers
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char *layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto &layerProperties : availableLayers)
			{
				if (layerName == layerProperties.layerName)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	/* ========== MAIN LOOP ========== */
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup()
	{
		vkDestroyInstance(instance, nullptr); // optional allocator callback ignored by passing nullptr

		glfwDestroyWindow(window);

		glfwTerminate();
	}
};

int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
