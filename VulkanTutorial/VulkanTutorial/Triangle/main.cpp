#include "HelloTriangleApp.h"

// Test vulkan is running correctly.
//void testVulkan() {
//	glfwInit();
//
//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);
//
//	uint32_t extensionCount = 0;
//
//	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//	std::cout << "Number of supported extensions : " << extensionCount << std::endl;
//
//	glm::mat4 mat = glm::mat4(1);
//	glm::vec4 vec = { 1,0,0,0 };
//	auto test = mat * vec;
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glfwPollEvents();
//	}
//
//	glfwDestroyWindow(window);
//
//	glfwTerminate();
//}

int main() 
{
	HelloTriangleApp app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}