#pragma once

// To access Windows native support.
//#define VK_USE_PLATFORM_WIN32_KHR
//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3native.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>

// Struct defining supported Queue Families.
struct QueueFamilyIndices
{
	// Graphics command support.
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;		/*Min/max number of images, width and  height of images*/
	std::vector<VkSurfaceFormatKHR> formats;	/*Surface formats (pixel format, color space)*/
	std::vector<VkPresentModeKHR> presentModes;	/*Available presentation modes*/
};

class HelloTriangleApp
{
public:
	void run()
	{
		#ifdef NDEBUG
		std::cout << "[DEBUG]: Validation layers enabled." << std::endl;
		#endif
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	
	// Variables
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	GLFWwindow* window;
	// Vulkan
	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device; // Logical device
	VkQueue graphicsQueue;
	VkQueue presentQueue; /*Presentation queue implies support for swapchains*/

	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages; /*Cleaned up once swapchain is destroyed*/
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	// Run functions
	void initWindow();
	void mainLoop();
	void cleanup();
	// Vulkan functions
	void initVulkan()
	{
		createInstance();
		createSurface();
		setupDebugMessenger();
		pickPhysicalDevice();
		CreateLogicalDevice();
		createSwapChain();
	}
	void createInstance();
	void pickPhysicalDevice();
	void CreateLogicalDevice();
	void createSwapChain();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createSurface();
	VkSurfaceFormatKHR chooseSwapSufraceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void cleanupVulkan();

	// Validation Layer
	#ifndef NDEBUG
	const bool enableValidationLayers = false;
	#else
	const bool enableValidationLayers = true;
	#endif
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
};

// Resource Allocation note
// We can use RAII (CADRe or SBRM)
// (https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization)
// or smart pointer for them.
// By implementing classes that
//	- get and release Vulkan objects in their constructor/destructor
//  - deleter for std::unique_ptr or std::shader_ptr