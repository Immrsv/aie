#pragma once


#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <vector>

class Model
{
public:
	Model();
	~Model();

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	
	void draw(unsigned int shaderID, glm::mat4 pvmMatrix);

	static Model* LoadModel(std::string file);

private:

	static std::map<std::string, Model*> m_models;

	struct OpenGLInfo {
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_faceCount;
	};

	std::vector<OpenGLInfo> m_glInfo;

	void createGLBuffers(tinyobj::attrib_t& attribs, std::vector<tinyobj::shape_t>& shapes);

	void load(const char* file);
};

