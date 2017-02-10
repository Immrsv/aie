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

	unsigned int m_shaderID;

	unsigned int ui_rows;
	unsigned int ui_cols;

	void load(const char* file);
	void generatePlane();

	void draw(glm::mat4 pvMatrix);

private:
	struct OpenGLInfo {
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_faceCount;
	};

	std::vector<OpenGLInfo> m_glInfo;

	void createGLBuffers(tinyobj::attrib_t& attribs, std::vector<tinyobj::shape_t>& shapes);
};

