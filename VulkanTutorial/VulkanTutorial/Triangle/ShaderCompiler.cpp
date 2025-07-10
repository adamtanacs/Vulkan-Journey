#include "ShaderCompiler.h"

std::vector<char> ShaderCompiler::readFile(const std::string& filename)
{
	std::ifstream file(filename, 
		std::ios::ate |		/* reading at the end of the file */
		std::ios::binary	/* use binary mode, avoid text transformations */
	);
	
	// Check if file opening was successful
	if (!file.is_open())
	{
		throw std::runtime_error("[ERROR] : Failed to open file!");
	}

	// Determine size of file, to allocate buffer for it
	// (thanks to std::ios::ate)
	size_t fileSize = (size_t)(file.tellg());
	std::vector<char> buffer(fileSize);

	// Read binary file content
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}