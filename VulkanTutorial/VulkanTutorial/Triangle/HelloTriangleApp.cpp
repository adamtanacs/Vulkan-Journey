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

void HelloTriangleApp::createGraphicsPipeline()
{
	// Graphics pipeline: sequence of operations, takes in vertices/textures and renders them
	// Stages: 
	// (P : programable, F : fixed stages) 
	// (Pipeline is from top-to-bottom)
	// Input assembler		(F) : collects raw vertex data, 
	//							  may use indices to avoid repeat vertex data
	// Vertex shader		(P)	: runs for every vertex, 
	//							  applies transformations (turn model space to screen space)
	//							  passes down per-vertex data down the pipeline
	// Tessalation shader	(P)	: subdivide geometry 
	// (optional)				  based on certain rules to increase mesh quality
	// Geometry shader		(P)	: runs for every primitive (tri, line, point),
	// (optional)				  discard/outputs more of them (similar to Tesselation shader)
	//							  not used much because of performance
	// Rasterization		(F) : discretizes primitives into fragments,
	//							  discards elements outside screen/during depth testing,
	//							  interpolates vertex attributes between fragments
	// Fragment shader		(P) : runs for every surviving fragments,
	// (optional)				  determines which framebuffer(s) are written to and color/depth values, 
	//							  interpolate between data from vertex shader (normals,texture coords)
	// Color blending		(F) : mix different fragments that map to the same pixel
	// 
	// Pipeline setting can be changed (just like in OGL)
	// Pipeline is immutable (means when doing changes, you need to do it from scratch again)

	// ---------------------------- SHADER MODULES ----------------------------
	// Define programable stages of the graphics pipeline

	// Reading pre-compiled SPIR-V shaders
	auto vertShaderCode = ShaderCompiler::readFile("vert.spv");
	auto fragShaderCode = ShaderCompiler::readFile("frag.spv");

	// Create shader modules for each shaders
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	// Define vertex shader stage of graphics pipeline
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; /* define it as vertex shader */
	vertShaderStageInfo.module = vertShaderModule;
	// Define entry point a.k.a "main" function
	vertShaderStageInfo.pName = "main";
	// Allows us to define constant values of shader constants
	// More effecient than using it at runtime
	// Enables optimalizations (eliminate "if"s)
	vertShaderStageInfo.pSpecializationInfo = nullptr;

	// Define fragment shader stage of graphics pipeline
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; /* define it as fragment shader */
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	// Collect the shader stage creation infos into an array
	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertShaderStageInfo,
		fragShaderStageInfo
	};

	// ------------------------- FIXED-FUNCTION STATE -------------------------
	// Define fixed stages of the graphics pipeline.

	// We can define dynamic states, 
	// which can be changed without recreating the pipeline at draw time.
	// However, now these values are required to be defined by us at draw time.
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,	/* region of framebuffer to be transformed */
		VK_DYNAMIC_STATE_SCISSOR	/* region of framebuffer to be filtered */
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	// Defining vertex input descriptor
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	// Defining vertex input attribute descriptor
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	// Define how the pipeline should handle the inputted vertices
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	// Define what kind of geometry should be drawn
	// VK_PRIMITIVE_TOPOLOGY_POINT_LIST		: points from vertices
	// VK_PRIMITIVE_TOPOLOGY_LINE_LIST		: line from every 2 vertices
	//										  without reuse
	// VK_PRIMITIVE_TOPOLOGY_LINE_STRIP		: end vertex is 
	//										  reused to start next line
	// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST	: triangle from every 3 vertices 
	//										  without reuse
	// VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP	: second and third vertices are 
	//										  reused for next triangle
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	// Defines whether to use a special vertex index value (0xFFFFFFFF or 0xFFFF)
	// Used for optimalisation to break up lines/triangles to _STRIP topologies
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Define viewport state for pipeline
	// Using multiple viewports/scissors requires enabling it at logical device creation
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount	= 1;	/* Number of viewports used */
	// viewportState.pViewports = &viewport;	// Only for static states
	viewportState.scissorCount	= 1;	/* Number of scissors used */
	// viewportState.pScissors = &scissor;		// Only for static states

	// Rasterizer: turns geometry into fragments for fragment shader
	// Also performs depth testing, face culling, scissor test, wireframe rendering
	// Define rasterization state for pipeline
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	// Clamp fragments beyond near/far values to them, needs to be enabled on GPU
	rasterizer.depthClampEnable = VK_FALSE;
	// Define how fragments are generated
	// VK_POLYGON_MODE_FILL		: fill area of polygon with fragments
	// VK_POLYGON_MODE_LINE		: polygon edges are drawn as lines
	// VK_POLYGON_MODE_POINT	: polygon vertices are drawn as points
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f; /* wider lines than 1.0f needs to be enabled on GPU */
	// Enable culling
	// cullMode : determines what type of face culling to use
	// VK_CULL_MODE_NONE			: no triangles discarded
	// VK_CULL_MODE_FRONT_BIT		: front-facing triangles are discarded
	// VK_CULL_MODE_BACK_BIT		: back-facing triangles are discarded
	// VK_CULL_MODE_FRONT_AND_BACK	: all triangles are discarded
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	// frontFace : specifies vertex order to be considered front-facing
	// VK_FRONT_FACE_COUNTER_CLOCKWISE	: triangle positive area is front-facing
	// VK_FRONT_FACE_CLOCKWISE			: triangle negative area is front-facing
	// positive/negative are calculation : https://registry.khronos.org/vulkan/specs/latest/man/html/VkFrontFace.html
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	// We can also add/biasing for depth values (example: shadow mapping)
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	// Multisampling: one way to perform anti-aliasing
	// Combines multiple polygons that rasterize to the same pixel
	// GPU feature needs to be enabled
	// (Will be revisited)
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	// Colorblending: combine with color value already on framebuffer
	// 2-ways: 
	// - mix old and new one
	// - combine old and new one 
	//	 (using bitwise operation)
	
	// Define configuration per attached framebuffers
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	// Which of the RGBA values are enabled for writing
	colorBlendAttachment.colorWriteMask = 
		VK_COLOR_COMPONENT_R_BIT | 
		VK_COLOR_COMPONENT_G_BIT | 
		VK_COLOR_COMPONENT_B_BIT | 
		VK_COLOR_COMPONENT_A_BIT;
	// Define which blend factor to use for source/destination RGB values
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	// Define which blend operation to use for calculating RGB values
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	// Define which blend factor to use for source/destination Alpha values
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	// Define which blend operation to use for calculating Alpha values
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	// All possible blend factors: https://registry.khronos.org/vulkan/specs/latest/man/html/VkBlendFactor.html
	// All possible blend operations : https://registry.khronos.org/vulkan/specs/latest/man/html/VkBlendOp.html
	// Pseudo-code for colorblend algorithm
	{
		//	if (blendEnable) {
		//		finalColor.rgb = (srcColorBlendFactor * newColor.rgb) <colorBlendOp> (dstColorBlendFactor * oldColor.rgb);
		//		finalColor.a = (srcAlphaBlendFactor * newColor.a) <alphaBlendOp> (dstAlphaBlendFactor * oldColor.a);
		//	}
		//	else {
		//		finalColor = newColor;
		//	}

		//	finalColor = finalColor & colorWriteMask;
	}

	// Example: alpha blending (with pseudo-code)
	{
		// colorBlendAttachment.blendEnable = VK_TRUE;
		// colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		// colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		// colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		// colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		// colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		// colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		// 
		// -------------------------Pseudo-code-------------------------
		// 
		// finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
		// finalColor.a = newAlpha.a;
	}

	// Define an array of structures for all framebuffers,
	// allows you to set blend constants
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	// Enable second method of blending (bitwise combination)
	// This will disable the first method for all framebuffers!
	// However: the colorWriteMask will be still used
	// (as if: blendEnable = VK_FALSE)
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	// Set blend constants
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	// --------------------------- PIPELINE LAYOUT ----------------------------
	// Define uniform and push values, referenced by shaders, updated at draw time
	
	// Create pipeline layout for our renderer
	// Note: uniform values needs to be defined here
	// Uniform values: dynamic state variables that can be changed at draw time
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create pipeline layout!");
	}

	// -------------------------- PIPELINE ASSEMBLY ---------------------------
	// Assemble our graphics pipeline to dispatch drawing operations

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// Define the programable stages of our pipeline
	pipelineInfo.stageCount = 2; /* vertex/fragment stages */
	pipelineInfo.pStages = shaderStages;
	// Define fixed-function stage of our pipeline, one-by-one
	pipelineInfo.pVertexInputState		= &vertexInputInfo;
	pipelineInfo.pInputAssemblyState	= &inputAssembly;
	pipelineInfo.pViewportState			= &viewportState;
	pipelineInfo.pRasterizationState	= &rasterizer;
	pipelineInfo.pMultisampleState		= &multisampling;
	pipelineInfo.pDepthStencilState		= nullptr;
	pipelineInfo.pColorBlendState		= &colorBlending;
	pipelineInfo.pDynamicState			= &dynamicState;
	// Define the pipeline layout
	pipelineInfo.layout = pipelineLayout;
	// Define render pass of our pipeline
	// Its possible to switch up the render pass of our pipeline
	// More about that: https://docs.vulkan.org/spec/latest/chapters/renderpass.html#renderpass-compatibility
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;	/* no subpasses */
	// Define pipeline by deriving it from an already existing one
	// Mainly for efficiency, when defining functionally similar pipelines
	// Can be done with handle or with index
	// Only used if:
	// pipelineInfo.flags = VK_PIPELINE_CREATE_DERIVATIVE_BIT;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(
		device, 
		VK_NULL_HANDLE, /* used to significantly speed up pipeline creation */
		1, 
		&pipelineInfo,  /* multiple pipelines can be created with a single call */
		nullptr, 
		&graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create graphics pipeline!");
	}

	// Cleaning up shader modules
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

VkShaderModule HelloTriangleApp::createShaderModule(const std::vector<char>& code)
{
	// Define shader module creation parameters
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	// Bytecode is specified in bytes, 
	// but pCode is const uint32*, 
	// therefore we need to recast it
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create shader module!");
	}

	return shaderModule;
}

void HelloTriangleApp::createRenderPass()
{
	// ----------------------------- RENDER PASS ------------------------------
	// Define attachments referenced by the pipeline stages and their usage
	
	// Define framebuffer attachments that will be used while rendering
	// Specify how many color/depth buffers there will be,
	// how many samples to use for each of them and how their contents
	// should be handled throughout the rendering operations
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;		/* swap chain image format */
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;	/* no multi sampling so its 1*/
	// Define what to do with COLOR and DEPTH data BEFORE rendering
	// VK_ATTACHMENT_LOAD_OP_LOAD		: preserve existing contest
	// VK_ATTACHMENT_LOAD_OP_CLEAR		: clear values to a constant at start
	// VK_ATTACHMENT_LOAD_OP_DONT_CARE	: existing contents are undefined
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// Define what to do with COLOR and DEPTH data AFTER rendering
	// VK_ATTACHMENT_STORE_OP_STORE		: store contents in memory for reading
	// VK_ATTACHMENT_STORE_OP_DONT_CARE	: contents of the framebuffer will be undefined
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	// Stencil : screen sized buffer, lets you mask off sections of the screen
	// Stencil and scissors are different
	// Stencil can map to any shape			- (per pixel)
	// Scissors can only map to rectangles	- (per polygon)
	// Define what to do with STENCIL data BEFORE rendering
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	// Define what to do with STENCIL data AFTER rendering
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// Define initial (before render pass) 
	// and final (after render pass) layout of framebuffer
	// All framebuffers are represented as VkImage objects
	// Most common layouts:
	// VK_IMAGE_LAYOUT_UNDEFINED				: we don't care about previous layout
	// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL	: for color attachment
	// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR			: to be presented in swap chain
	// VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL		: as destination for memory copy operation
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// One render pass can consist of multiple subpasses
	// Every subpass references one or more attachments (described in this struct)
	VkAttachmentReference colorAttachmentRef{};
	// Specifies which attachments to reference by its index
	// Referenced in fragment shader: layout(location = 0) out vec4 outColor
	colorAttachmentRef.attachment = 0;	/* reference first element */
	// Specifies which layout to have during a subpass (automatically transitions to it)
	// For all VkImage layouts : https://registry.khronos.org/vulkan/specs/latest/man/html/VkImageLayout.html
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Define subpass details
	VkSubpassDescription subpass{};
	// Define it as graphics subpass
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	// Other attachments that can be referenced by a subpass
	// subpass.pInputAttachments		: read from a shader
	// subpass.pResolveAttachments		: used for multisampling color attachments
	// subpass.pDepthStencilAttachment	: used for depth and stencil data
	// subpass.pPreserveAttachments		: not used by subpass but MUST be preserved

	// ------------------------- SUBPASS DEPENDENCE ---------------------------
	// Subpass dependencies : specify memory and execution 
	//						  dependencies between subpasses
	// 2 implicit dependency definitions are built in: start and end of render pass
	// We need to create one, because 
	// the built-in dependency for the end of render pass is incorrect
	// Subpass dependency solution #2 : change render pass wait for the
	//									to VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	VkSubpassDependency dependency{};
	// Specify indices of the dependency and dependent subpass
	// srcSubpass must always we higher then dstSubpass
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;	/* special value for implicit 
													   subpass before/fater render pass 
													   depending if its src/dst */
	dependency.dstSubpass = 0;						/* reference to our only subpass */
	// Specify operations to wait on and stages on which these occur
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	// Specify operations that other subpasses have to wait on
	// and stages on which these occur
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	// Creating our render pass
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	// Adding subpass dependencies
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create render pass!");
	}
}

void HelloTriangleApp::createFramebuffers()
{
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); ++i)
	{

		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		// Define framebuffer properties
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		// Define which render pass does it belong to
		framebufferInfo.renderPass = renderPass;
		// Define VkImageView object for the framebuffer
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		// Define framebuffer size (equal to swap chain extent)
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		// Define how many layers does our swap chain image consts of.
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] : Failed to create framebuffer!");
		}
	}
}

void HelloTriangleApp::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

	// Define command pool properties
	VkCommandPoolCreateInfo poolInfo{};
	// Define usage behaviour through a bitmask
	// VK_COMMAND_POOL_CREATE_TRANSIENT_BIT				: short-lifetime,
	//													  will be reseted/freed often
	// VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT	: allows to be individially 
	//													  reseted to initial state
	// VK_COMMAND_POOL_CREATE_PROTECTED_BIT				: allocated command buffers 
	//													  are protected command buffers
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	// Set (graphics) queue family to be used for creation
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to create command pool!");
	}
}

void HelloTriangleApp::createCommandBuffer()
{
	// Create multiple command buffers
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	// Define command buffer properties for allocation
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	// Define which command pool will it belong to
	allocInfo.commandPool = commandPool;
	// Define if its:
	// VK_COMMAND_BUFFER_LEVEL_PRIMARY		: can be submited to a queue for execution
	//										  cannot be called from other buffers
	// VK_COMMAND_BUFFER_LEVEL_SECONDARY	: cannot be sumbitted directly, but
	//										  can be called from primary command buffers
	// (used for common operation exections)
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	// Define number of command buffers to allocate
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("[ERROR] : Failed to allocate command buffers!");
	}
}

void HelloTriangleApp::createSyncObjects()
{
	// Create multiple semaphores/fences
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	// Define properties for the creation of semaphores
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Define propreties for the creation of fences
	// First draw frame call is also waiting for fence signal
	// So we need to create it in singaled state
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;	/* create fence in signaled state */

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] : Failed to create semaphores!");
		}

		if (vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("[ERROR] : Failed to create fences!");
		}
	}
}

void HelloTriangleApp::recordCommandBuffer(
	VkCommandBuffer commandBuffer,	/* command buffer to allocate commands */
	uint32_t imageIndex				/* index of current swap chain image */
)
{
	// Begin recording commands by calling vkBeginCommandBuffer
	// Implicitly resets command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// Define how we want to use the command buffer
	// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT		: will be rerecorded,
	//													  right after execution
	// VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT	: secondary command buffer,
	//													  entirely within a single render pass
	// VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT		: will be resubmitted,
	//													  while it also pending execution
	beginInfo.flags = 0;
	// Specify which state to inherite from the calling primary buffer
	// Only relevant for secondary buffers
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("[ERROR] : Failed to begin recording command buffer!");
	}

	// Define begin render pass properties
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
	// Define render area offset/extent
	// Match size of attachments for best performance
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapChainExtent;
	// Define clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Begin render pass and also start drawing

	vkCmdBeginRenderPass(
		commandBuffer,				/* command buffer to record to */
		&renderPassInfo,			/* details of the render pass */
		// How the draw commands are provided
		// Possible two values for this:
		// VK_SUBPASS_CONTENTS_INLINE : render pass commands are embedded in the 
		//								primary command buffer, 
		//								no secondary command buffer will be executed
		// VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS : render pass commands will be
		//												   executed from 
		//												   secondary command buffers
		VK_SUBPASS_CONTENTS_INLINE
	);
	{
		vkCmdBindPipeline(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS, /* define pipeline object type */
			graphicsPipeline
		);

		// Because we defined viewport/scissors to be dynamic
		// We have to provide them here

		// Define viewport for rendering
		// Viewport defines transformations from image to framebuffer
		// Almost always (0,0) to (width,height)
		VkViewport viewport{};
		// Set offset of viewport
		viewport.x = 0.0f;
		viewport.y = 0.0;
		// Set size of viewport
		// Remember : 
		// swapchain width/height NOT EQUAL width/height of window
		viewport.width = static_cast<float>(swapChainExtent.width);
		viewport.height = static_cast<float>(swapChainExtent.height);
		// Set depth value range
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(
			commandBuffer,
			0,	/* index of first viewport to be updated */
			1,	/* number of viewports to be updated */
			&viewport
		);

		// Define scissor for rendering
		// Scissor defines which regions/pixels will actually be stored 
		// outside pixels will be discarded (filters them out)
		VkRect2D scissor{};
		scissor.offset = { 0,0 };
		scissor.extent = swapChainExtent;

		vkCmdSetScissor(
			commandBuffer,
			0,	/* index of first scissor to be updated */
			1,	/* number of scissors to be updated */
			&scissor
		);

		vkCmdDraw(
			commandBuffer,
			3,	/* number of vertices to draw */
			1,	/* number of instances to draw, used for instanced rendering */
			0,	/* offset into the vertex buffer,
				   defines lowest value of vertex index */
			0	/* offset for instanced rendering,
				   defines lowest value of instanced index */
		);
	}
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to record command buffer!");
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
		drawFrame();
	}
	// Wait for logical device to finish all operations
	vkDeviceWaitIdle(device);
}

void HelloTriangleApp::drawFrame()
{
	// At a high level:
	// 1. Wait for the prev. frame to finish
	// 2. Acquire an image from the swap chain
	// 3. Record a command buffer (draws the scene onto an image)
	// 4. Submit recorded command buffer
	// 5. Present swap chain image

	// Synchronization of execution on the GPU in Vulkan is explicit
	// Semaphores	: keep the order of GPU operations sychronized ** PREFERED **
	// Fences		: keep CPU and GPU in sych with each other
	// ------------------------------------------------------------------------------
	// Semaphores	: used for ordering between queue operations on the GPU (device),
	//				  2 kinds: binary and timeline (we only use binary here)
	//				  (queue operations are the ones 
	//				  that are submitted through command buffers)
	//				  the waiting only happens on the GPU, CPU returns immedietly
	//				  they have 2 states: signaled and unsignaled
	// 
	// We use semaphores in 2-ways: for signaling in one queue and waiting in another
	// Example:
	// 
	// VkCommandBuffer A, B;
	// VkSemaphore S;
	// 
	// // enqueue A, signal S when done - starts executing immediately
	// vkQueueSubmit(work: A, signal : S, wait : None);
	// 
	// enqueue B, wait on S to start
	// vkQueueSubmit(work: B, signal : None, wait : S);
	// 
	// ------------------------------------------------------------------------------
	// Fences		: used for ordering between queue operations on the CPU (host),
	//				  attach it to a queue operation and signal it when it finishes,
	//				  while we wait on the CPU for the signaled state
	//				  must be reset manually
	//				  (similar to semaphores)
	// Example:
	// 
	// VkCommandBuffer A;
	// VkFence F;
	// 
	// // enqueue A, start work immediately, signal F when done
	// vkQueueSubmit(work: A, fence : F);
	// 
	// // blocks execution until A has finished executing
	// vkWaitForFence(F);
	// // CPU operation, can't run until the transfer has finished
	// save_screenshot_to_disk();

	// Wait for the previous frame to render
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	// Reset fences
	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	// Reset command buffer to starting state
	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	
	// Record our commands in the command buffer
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	// Submit commands onto the GPU
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	
	// Which semaphores we have to wait for before execution
	// Subpass dependency solution #1	: change imageAvailableSemaphore 
	//									  to VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame]};
	// Which stages of the pipeline do we have to wait for
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	// submitInfo.pWaitSemaphores = &imageAvailableSemaphore;	// Also valid
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	// Which semaphores we have to signal after exection
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame]};
	// Which semaphores to signal when finished
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	// submitInfo.pSignalSemaphores = &renderFinishedSemaphore;	// Also valid

	// Submit command buffer on the graphics queue
	// We also use inFlightFence to signal 
	// when we can reuse the command buffer
	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("[ERROR] : Failed to submit draw command buffer!");
	}

	// Present the swap chain image to the screen
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	// Specify which semaphores to wait on before presentation
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	// Specify which swap chain to take our image from
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	// And which image from the swap chain to take
	presentInfo.pImageIndices = &imageIndex;
	// We can also check each and every swap chain presentations were successful
	VkResult res;
	presentInfo.pResults = &res;

	vkQueuePresentKHR(presentQueue, &presentInfo);
	
	// Advance frame counter
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApp::cleanupVulkan()
{
	// Destroy synchronization objects
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT;++i)
	{
		// Semaphores
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		// Fences
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	// Destroy command pool
	vkDestroyCommandPool(device, commandPool, nullptr);

	// Destroy framebuffer objects
	for (auto framebuffer : swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	// Destroy (grahpics) pipeline
	vkDestroyPipeline(device, graphicsPipeline, nullptr);

	// Destroy pipeline layout
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

	// Destroy render pass
	vkDestroyRenderPass(device, renderPass, nullptr);

	// Destroy image views
	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	// Destroy swap chain
	vkDestroySwapchainKHR(device, swapChain, nullptr);

	// Destroy logical device.
	vkDestroyDevice(device, nullptr);

	// Destroy Debug messenger object
	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

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
