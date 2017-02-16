#pragma once
#include <string>
#include <map>

class Shader
{
public:
	static unsigned int CompileShaders(std::string name, char* vsFile, const char* fsFile);

	static unsigned int GetProgramID(std::string name);


	
private:
	static bool LoadShaderFromFile(const char* filePath, std::string& code);

	static std::map<std::string, unsigned int> collection;
};