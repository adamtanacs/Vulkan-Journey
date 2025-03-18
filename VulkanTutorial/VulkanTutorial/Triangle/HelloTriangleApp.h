#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

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
	
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	// Variables
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	GLFWwindow* window;
	VkInstance instance;

	// Validation Layer
	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};
	#ifndef NDEBUG
	const bool enableValidationLayers = false;
	#else
	const bool enableValidationLayers = true;
	#endif
	bool checkValidationLayerSupport();
	VkDebugUtilsMessengerEXT debugMessenger;
	void setupDebugMessenger();

	std::vector<const char*> getRequiredExtensions();

	// Run functions
	void initWindow();
	void initVulkan();
	void createInstance();
	void mainLoop();
	void cleanupVulkan();
	void cleanup();
};

// Resource Allocation note
// We can use RAII (CADRe or SBRM)
// (https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization)
// or smart pointer for them.
// By implementing classes that
//	- get and release Vulkan objects in their constructor/destructor
//  - deleter for std::unique_ptr or std::shader_ptr