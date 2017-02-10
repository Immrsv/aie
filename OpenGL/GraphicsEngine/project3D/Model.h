#pragma once

#include <glm/glm.hpp>

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

	void draw(glm::mat4 pvMatrix);


};

