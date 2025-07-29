#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>

/// <summary>
/// Vertex containing the following data:
/// Position (float2) and Color (float3)
/// </summary>
struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions();
};