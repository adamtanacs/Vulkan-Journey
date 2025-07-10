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

void HelloTriangleApp::CreateLogicalDevice()
{
	// Logical Device : interface for physical device

	// Create queues to use
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
	std::set<uint32_t> uniqueQueueFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};
	
	// Assing priority to queue (even for single one).
	float queuePriority = 1.0f;

	// Creating queue creation infomations for each queue families.
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		// Creation info for one queue family.
		VkDeviceQueueCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		createInfo.queueFamilyIndex = queueFamily;
		createInfo.queueCount = 1;
		createInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(createInfo);
	}

	// Specify device features to use.
	VkPhysicalDeviceFeatures deviceFeatures{};

	// Logical device creation infos.
	// Using multiple queueFamilies.
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	// Enable extensions.
	// Note: these are device specific.
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// Enable Validation layers.
	// Note: older versions of Vulkan made 
	// instance and device Validation layer creation distinct.
	// This is no longer the case, but its good practice to set them anyway
	// to be compatible with older versions of Vulkan.
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	// Create logical device.
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create logical device!");
	}

	// Retrieve queue handles.
	{
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}
}

void HelloTriangleApp::createSwapChain()
{
	// Get needed properties for swapchain creation
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSufraceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	// Decide how many images we want to use in our swapchain
	// Minimum plus one (to avoid waiting for driver internal operations)
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	
	// Also check if not exceeding maximum amount of images
	// (value of 0 means there is no maximum)
	if (swapChainSupport.capabilities.maxImageCount > 0 &&
		imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Creating a (familiar) info struct for swapchain creation
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	// Specifying details of swap chain images
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; /* number of views for multiview/stereo surface */
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	// If you want to render different values to different images for post processing
	// createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	// Define how to handle swap chain images between multiple queue families
	// We are drawing images from the graphics queue
	// and submitting it to the presentation queue
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
	if (indices.graphicsFamily != indices.presentFamily)
	{
		// Image can be used across multiple families without explicit ownership transfers
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		// Must define: number of queue families
		createInfo.queueFamilyIndexCount = 2;
		// and the queue families which share ownership
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		// Image ownership must be explicitly transferred before usage
		// Offers best performance
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;		// Optional
		createInfo.pQueueFamilyIndices = nullptr;	// Optional
	}

	// Define current transformation (to specify that no transformations are needed)
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	// Otherwise can use transforms listed in 
	// swapChainSupport.capabilities.supportedTransforms

	// Define if alpha channel should be used for blending with other windows
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	// Discard rendering operations for obscured parts of window
	createInfo.clipped = VK_TRUE;

	// Exchange old/invalid/unoptimized swap chain 
	// by creating a new one and point to the old one
	// (For example: when resizing window)
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create swap chain!");
	}

	// Retrieve handles to Images of the swap chain
	// (Reuse imageCount uint32_t)
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr); /*can be more then min*/
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	// Store format and extent of our swap chain for future use
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void HelloTriangleApp::createImageViews()
{
	// VkImageView : object which describes how to access an image and which part to access
	
	// Create view for each and every swap chain image
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); ++i)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];

		// Specify how the image data should be interpreted
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; /* 1D,2D or 3D texture */
		createInfo.format = swapChainImageFormat;

		// Option to swizzle around color channels or map constant values (0 or 1)
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		// Describe what the image's purpose is and which part should be accessed
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0; /* Mipmap level first to be accessed */
		createInfo.subresourceRange.levelCount = 1; /* No mipmapping levels */
		// These are only used for stereographic 3D applications
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] : Failed to create image views!");
		}
	}
}

bool HelloTriangleApp::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// Enumerate all the available extensions
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,nullptr);

	// List of all physical device extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	// and check if the required extension is among them
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
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

	// Check for device extensions (swapchain support, etc.)
	bool extensionSupported = checkDeviceExtensionSupport(device);

	// Check for sufficient swapchain support
	bool swapChainAdequete = false;
	if (extensionSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequete = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionSupported && swapChainAdequete;
}

void HelloTriangleApp::createSurface()
{
	// Initialize window surface.
	// NOTE : this can be used with platform specific extension on GLFW.
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create window surface!");
	}
}

VkSurfaceFormatKHR HelloTriangleApp::chooseSwapSufraceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	// VkSurfaceFormat contains a format and colorspace
	// Format		: specifies color channels and types
	// Colorspace	: is SRGB color space supported (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
	//				  (in older versions its called VK_COLORSPACE_SRGB_NONLINEAR_KHR)
	//				  (results inmore accurate perceived colors)

	// Check for a specific format combination inside the list
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	// If it fails, settle for the first 
	// combination inside the list
	return availableFormats[0];
}

VkPresentModeKHR HelloTriangleApp::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	// VkPresentModeKHR describes conditions of showing images to the screen
	// 4 possible modes
	// VK_PRESENT_MODE_IMMEDIATE_KHR	: images are sent to screen right away
	//									  (may result in tearing)
	// VK_PRESENT_MODE_FIFO_KHR			: swap chain is a queue, takes image from the front to display,
	//									  new rendered images added to the back,
	//									  if the queue is full, the program has to wait
	//									  (similar to vsync, "vertical blank")
	// VK_PRESENT_MODE_FIFO_RELAXED_KHR	: similar to VK_PRESENT_MODE_FIFO_KHR
	//									  only differnce: doesn't wait for program to finish a frame,
	//									  when the queue is empty and the program is late
	//									  (may result in tearing)
	// VK_PRESENT_MODE_MAILBOX_KHR		: similar to VK_PRESENT_MODE_FIFO_KHR
	//									  commonly known as "tripple buffering"
	//									  when queue is full, replaces already existing images in queue with new ones
	//									  resulting in fewer latencies without tearing
	//									  (better than vsync)
	// ("Vertical blank": the moment the display is refreshed)

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	// Only VK_PRESENT_MODE_FIFO_KHR is guaranteed
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D HelloTriangleApp::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// Resolution of swap chain images (almost always equal with resolution of window)
	// Defined by VkSurfaceCapabilitiesKHR struct
	// Usually try to match with currentExtent size
	// However, some window managers allow to differ, 
	// indicated by value of uint32_t for currentExtent member width and height values
	// In that case: pick best option between minImageExtent and maxImageExtent

	// CAUTION
	// GLFW: uses 2 metrics (pixels,screen coords.), may differ based on DPI
	// Previous resolution setting was in SCREEN COORDS

	// Check if extent needs to be matched
	if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
	{
		// Get GLFW frame size in PIXELS
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Create struct for actual extent
		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		// Clamp width/height between min and max values
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.width = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}

	// Return current extent if we want to match value
	return capabilities.currentExtent;
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
	// NOTE : this implementation may return different queuefamily indices for different families.
	int i = 0;
	for (const auto queueFamily : queueFamilies)
	{
		// Find a queue family that supports graphics commands.
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		// Check if queue family supports presenting to our window surface.
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		
		if (presentSupport)
		{
			indices.presentFamily = i;
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

SwapChainSupportDetails HelloTriangleApp::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	// Get surface capabilities
	// Taking into account physical device and window surface
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// Get number of supported surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	// Querying supported surface formats
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// Get the number of supported surface presentation modes
	uint32_t presentationCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);

	// Querying supported surface presentation modes
	if (presentationCount != 0)
	{
		details.presentModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, details.presentModes.data());
	}

	return details;
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

	// Destroy image views
	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	// Destroy swap chain
	vkDestroySwapchainKHR(device, swapChain, nullptr);

	// Destroy logical device.
	vkDestroyDevice(device, nullptr);

	// Destroy window surface.
	vkDestroySurfaceKHR(instance, surface, nullptr);

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
