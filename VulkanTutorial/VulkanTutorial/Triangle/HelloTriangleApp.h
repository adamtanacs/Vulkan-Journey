#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>

// Struct defining supported Queue Families.
struct QueueFamilyIndices
{
	// Graphics command support.
	std::optional<uint32_t> graphicsFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value();
	}
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
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device; // Logical device
	VkQueue graphicsQueue;

	// Run functions
	void initWindow();
	void mainLoop();
	void cleanup();
	// Vulkan functions
	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		pickPhysicalDevice();
		CreateLogicalDevice();
	}
	void createInstance();
	void pickPhysicalDevice();
	void CreateLogicalDevice();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
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