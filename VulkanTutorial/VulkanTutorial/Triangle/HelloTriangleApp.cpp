#include "HelloTriangleApp.h"
#include <vector>

void HelloTriangleApp::initWindow()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW its not OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Tell GLFW its NOT resizeable
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create a window in GLFW
	// glfwCreateWindow(
	// {width},
	// {height},
	// {title of window},
	// {specify monitor to open on},
	// {only relevant for OpenGL})
	window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan",nullptr,nullptr);
}

void HelloTriangleApp::initVulkan()
{
	createInstance();
}

void HelloTriangleApp::createInstance()
{
	// List supported extensions
	{
		// Retrieve number of extensions.
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		
		// Query extension details.
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "Available extensions:" << std::endl;

		for (const auto& ext : extensions)
		{
			std::cout << '\t' << ext.extensionName << " version: " << ext.specVersion << std::endl;
		}
	}
	// Create instance
	{
		VkApplicationInfo appInfo{};

		// Define structure.
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		// Name of the application
		appInfo.pApplicationName = "Hello Triangle";
		// Developer supplied version number.
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		// Name of (if used) the engine used to create the application.
		appInfo.pEngineName = "No Engine";
		// Developer supplied version number.
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		// Define version of Vulkan the application will use.
		// MUST be the highest version it will use.
		appInfo.apiVersion = VK_API_VERSION_1_0;
		// Pointer for extension information.
		// appInfo.pNext = nullptr;

		// Required struct to create our instance.
		// Tells Vulkan which global extensions and validation layers to use.
		// Global = Entire program
		VkInstanceCreateInfo createInfo{};
		// Define structure.
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		// Pointer to application information (defined before this).
		createInfo.pApplicationInfo = &appInfo;

		// Create GLFW extension(s).
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// Pass through extension(s) required by window system.
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		// Enable global validation layers.
		createInfo.enabledLayerCount = 0;

		// Create Instance.
		// Pattern of object creation
		//	- pointer to creation info
		//	- pointer to callbacks
		//	- pointer to variable to store handle to new object
		// VkResult is either VK_SUCCESS or error code
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance!");
		}
	}
	
	// VkAllocationCallbacks
	// pointer to callback functions for memory allocations.
}

void HelloTriangleApp::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		// GLFW looking for events to handle
		glfwPollEvents();
	}
}

void HelloTriangleApp::cleanupVulkan()
{


	// Destroy instance last
	vkDestroyInstance(instance, nullptr);
}

void HelloTriangleApp::cleanup()
{
	cleanupVulkan();

	// Destroy created window(s)
	glfwDestroyWindow(window);

	// Terminate GLFW
	glfwTerminate();
}
