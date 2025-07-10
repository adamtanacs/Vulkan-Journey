#pragma once

#include <fstream>
#include <vector>
#include <filesystem>

class ShaderCompiler
{
private:

public:
	static std::vector<char> readFile(const std::string& filename);
};

