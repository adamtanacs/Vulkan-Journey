#include "HelloTriangleApp.h"

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
}

void HelloTriangleApp::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		// GLFW looking for events to handle
		glfwPollEvents();
	}
}

void HelloTriangleApp::cleanup()
{
	// Destroy created window(s)
	glfwDestroyWindow(window);

	// Terminate GLFW
	glfwTerminate();
}
