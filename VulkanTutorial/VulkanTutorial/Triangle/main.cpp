#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <iostream>

int main() 
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

	uint32_t extensionCount = 0;

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << "Number of supported extensions : " << extensionCount << std::endl;

	glm::mat4 mat = glm::mat4(1);
	glm::vec4 vec = { 1,0,0,0 };
	auto test = mat * vec;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}