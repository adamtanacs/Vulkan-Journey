#include "HelloTriangleApp.h"

// Callback function defined to handle messages from Vulkan Validation layer
// PFN_vkDebugUtilsMessengerCallbackEXT prototype
VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApp::debugCallback(
	// Bitmask specifying the severity of event that caused the callback
	// Severity Types:
	// - VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT :		Diagnostic
	// - VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT :			Informational
	// - VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT :		Problematic (bug) behaviour
	// - VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT :		Fatal
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	// Bitmask specifying which event(s) triggered the callback
	// Msg Types:
	// - VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT :			General
	// - VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:		Specification
	// - VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:		Performance
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	// Structure specifying all parameters for callback data
	// - pMessage :		Debug Msg
	// - pObjects :		Related Vulkan obj. handles
	// - object count:	number of objects in pObjects
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	// User-provided data at the creation of 
	// VkDebugUtilsMessengerEXT (handle to a debug messenger)
	void* pUserData)
{
	// Can be used with comp. operator
	//if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
	//	// Message is important enough to show
	//	std::cerr << "[VALIDATION LAYER]: " << pCallbackData->pMessage << std::endl;
	//}

	const char* severity;
	switch (messageSeverity)
	{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			severity = "DIAGNOSTIC";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			severity = "INFO";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			severity = "WARNING";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			severity = "ERROR";
			break;
		default:
			severity = "UNKNOWN";
			break;
	}

	if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		return VK_FALSE;

	std::cerr << "[VALIDATION LAYER | " << severity
			  << "] : " << pCallbackData->pMessage << std::endl;
	//return VK_TRUE;	// ABORT WITH ERROR
	return VK_FALSE;	// ALWAYS
}

VkResult CreateDebugUtilsMessengerEXT(
	// Vulkan instance
	VkInstance instance,
	// Creation infomations
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	// Pointer to mem allocation function
	const VkAllocationCallbacks* pAllocator,
	// Pointer to messenger object
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func =
		// Pass to object creation function
		(PFN_vkCreateDebugUtilsMessengerEXT)
		// Look up instance address
		vkGetInstanceProcAddr(
			instance,
			"vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(
	// Vulkan instance
	VkInstance instance,
	// Messenger object
	VkDebugUtilsMessengerEXT debugMessenger,
	// Pointer to mem allocation function
	const VkAllocationCallbacks* pAllocator) {
	auto func = 
		// Pass to object creation function
		(PFN_vkDestroyDebugUtilsMessengerEXT)
		// Look up instance address
		vkGetInstanceProcAddr
		(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

bool HelloTriangleApp::checkValidationLayerSupport()
{
	// Return number of global layer properties available
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

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
	setupDebugMessenger();
	pickPhysicalDevice();
}

void HelloTriangleApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	// Define structure type
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	// Specify all the severities callback to be called for
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	// Specify all the msg types your callback to be called for
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	// Pointer to callback function
	createInfo.pfnUserCallback = debugCallback;
	// Optional
	createInfo.pUserData = nullptr;
}

void HelloTriangleApp::setupDebugMessenger()
{
	if (!enableValidationLayers) return;

	// Creation infomations
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	// For more info: 
	// https://docs.vulkan.org/spec/latest/chapters/debugging.html#VK_EXT_debug_utils

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) 
	{
		throw std::runtime_error("[ERROR] : Failed to set up debug messenger!");
	}
}

std::vector<const char*> HelloTriangleApp::getRequiredExtensions()
{
	// Get GLFW extension(s).
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// Setup debug messenger for callbacks
	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}
void HelloTriangleApp::createInstance()
{
	// Validation Layers
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("[ERROR]: Validation layers requested, but not available!");
		}
	}

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
			std::cout << "[EXTENSION]: " 
					  << ext.extensionName 
					  << "\t [VERSION]: "
					  << ext.specVersion
					  << std::endl;
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

		// Get extensions for the application
		std::vector<const char*> extensions = getRequiredExtensions();

		// Pass through extension(s) required by window system.
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Enable global validation layers.
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = 
				(VkDebugUtilsMessengerCreateInfoEXT*)
				&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}

		// Create Instance.
		// Pattern of object creation
		//	- pointer to creation info
		//	- pointer to callbacks
		//	- pointer to variable to store handle to new object
		// VkResult is either VK_SUCCESS or error code
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] : Failed to create instance!");
		}
	}
	
	// VkAllocationCallbacks
	// pointer to callback functions for memory allocations.
}

void HelloTriangleApp::pickPhysicalDevice()
{
	// Enumerate all available graphics cards with Vulkan support.
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("[ERROR] : Failed to find GPUs with Vulkan support!");
	}

	// Hold all the handles of supported GPUs.
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// Select first device suitable to use for Vulkan.
	// Vulkan also allows to make
	// a sort of "highest score" selections system too.
	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("[ERROR] : Failed to find a suitable GPU!");
	}
}

bool HelloTriangleApp::isDeviceSuitable(VkPhysicalDevice device)
{
	// Get basic device properties 
	// (Name, type, supported Vulkan version)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// Get optional funcionality support.
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Check if supports certain queue families
	QueueFamilyIndices indices = findQueueFamilies(device);

	return indices.isComplete();
}

QueueFamilyIndices HelloTriangleApp::findQueueFamilies(VkPhysicalDevice device)
{
	// Queue
	// Contains commands, texture uploads and drawcalls that needs to be executed
	// Queue families
	// Divide queues by functionality (example: compute calls, memory transfare)

	QueueFamilyIndices indices;

	// Get queue families.
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Find queue families that support certain features.
	int i = 0;
	for (const auto queueFamily : queueFamilies)
	{
		// Find a queue family that supports graphics commands.
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		// Early exit if we have support for all funcionalities.
		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
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
	// Destroy Debug messenger object
	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	// Destroy instance last
	// Also destroy VkPhysicalDevice impicitly.
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
