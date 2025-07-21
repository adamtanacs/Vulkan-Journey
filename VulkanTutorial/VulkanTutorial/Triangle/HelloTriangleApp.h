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

#include "ShaderCompiler.h"
#include "Vertex.h"

// Maximum number of frames rendered simultaneously
const int MAX_FRAMES_IN_FLIGHT = 2;

// Struct defining supported Queue Families.
struct QueueFamilyIndices
{
	// Graphics command support.
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() &&
			presentFamily.has_value() &&
			transferFamily.has_value();
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
	VkQueue presentQueue; /* Presentation queue implies support for swapchains */
	VkQueue transferQueue; /* Buffer data transfer queue */

	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages; /* Cleaned up once swapchain is destroyed */
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;	/* Manange memory of command buffers */
	VkCommandPool transferCommandPool; /* Manage memory of buffers */
	// NOTE : Cleaned up once command pool is destroyed
	std::vector<VkCommandBuffer> commandBuffers; 
	VkClearValue clearColor = { 
		{
			{ 0.125f, 0.25f, 0.5f, 1.0f }
		} 
	};
	uint32_t currentFrame = 0;

	// ------------------------- VERTICES --------------------------

	const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f},	{1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f},	{0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f},	{0.0f, 0.0f, 1.0f}}
	};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	// ------------------ SYNCHRONIZATION OBJECTS ------------------
	
	// image is acquired from the swap chain
	std::vector<VkSemaphore> imageAvailableSemaphores;
	// rendering of image finished, presentable
	std::vector<VkSemaphore> renderFinishedSemaphores;
	// one frame is rendered at the time
	std::vector<VkFence> inFlightFences;

	bool framebufferResized = false;

	// ----------------------- APP FUNCTIONS -----------------------
	
	void initWindow();
	void mainLoop();
	void drawFrame();
	void cleanup();
	
	// ---------------------- EVENT FUNCTIONS ----------------------
	
	// Callback to handle resize event
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	
	// --------------------- VULKAN FUNCTIONS ----------------------
	
	void initVulkan()
	{
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createVertexBuffer();
		// oldCreateVertexBuffer();
		createCommandBuffer();
		createSyncObjects();
	}
	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void recreateSwapChain();
	// Cleanup swap chain and all associated objects (framebuffers, imageviews)
	void cleanupSwapChain();
	void createImageViews();
	void createGraphicsPipeline();
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool();
	void oldCreateVertexBuffer();
	void createVertexBuffer();
	void createCommandBuffer();
	void createSyncObjects();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	VkShaderModule createShaderModule(const std::vector<char>& code);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
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