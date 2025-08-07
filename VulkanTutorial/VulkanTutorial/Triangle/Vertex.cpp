#include "Vertex.h"

VkVertexInputBindingDescription Vertex::getBindingDescription()
{
	// Vertex binding descriptions tell us how to ise
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0; // Index of binding in the array
	bindingDescription.stride = sizeof(Vertex); // Number of bytes between entries.
	// Define when to move to next data entry.
	// VK_VERTEX_INPUT_RATE_VERTEX		: after each vertex
	// VK_VERTEX_INPUT_RATE_INSTANCE	: after each instance (instanced rendering)
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> Vertex::attributeDescriptions()
{
	// Attribute descriptions help us find attributes from the vertex struct
	// We have 2 attributes so we need to attribute descriptions
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

	// Position
	attributeDescriptions[0].binding	= 0; // Which binding the per-vertex data comes from
	attributeDescriptions[0].location	= 0; // location, referenced inside the shaders
	// Format of our attributes, most common ones:
	// float	: VK_FORMAT_R32_SFLOAT
	// vec2		: VK_FORMAT_R32G32_SFLOAT
	// vec3		: VK_FORMAT_R32G32B32_SFLOAT
	// vec4		: VK_FORMAT_R32G32B32A32_SFLOAT
	attributeDescriptions[0].format		= VK_FORMAT_R32G32B32_SFLOAT;
	// Offset of our attribute inside the vertex struct
	attributeDescriptions[0].offset		= offsetof(Vertex, pos);

	// Color
	attributeDescriptions[1].binding	= 0;
	attributeDescriptions[1].location	= 1;
	attributeDescriptions[1].format		= VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset		= offsetof(Vertex,color);

	// Texture coordinates
	attributeDescriptions[2].binding	= 0;
	attributeDescriptions[2].location	= 2;
	attributeDescriptions[2].format		= VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset		= offsetof(Vertex,texCoord);

	return attributeDescriptions;
}

bool Vertex::operator==(const Vertex& other) const
{
	return
		pos == other.pos &&
		color == other.color &&
		texCoord == other.texCoord;
}