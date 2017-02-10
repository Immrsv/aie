#include "Model.h"

#include <glm/ext.hpp>
#include <gl_core_4_4.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex {
	glm::vec4 position;
	glm::vec4 color;
};


Model::Model()
{
}


Model::~Model()
{
}

void Model::load(const char* file) {

	unsigned int rows = ui_rows = 10;
	unsigned int cols = ui_cols = 10;

	Vertex* aoVertices = new Vertex[rows*cols];
	for (unsigned int r = 0; r < rows; ++r) {
		for (unsigned int c = 0; c < cols; ++c) {
			aoVertices[r*cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));

			aoVertices[r*cols + c].color = vec4(color, 1);
		}
	}

	unsigned int* auiIndices = new unsigned int[(rows - 1)*(cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r) {
		for (unsigned int c = 0; c < (cols - 1); ++c) {
			// triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			// tiangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}


	glGenVertexArrays(1, &m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows*cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1)*(cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] aoVertices;
}
void Model::draw(glm::mat4 pvMatrix) {

	glUseProgram(m_shaderID);

	unsigned int projectionViewUniform = glGetUniformLocation(m_shaderID, "projectionViewWorldMatrix");

	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(pvMatrix));
	glBindVertexArray(m_VAO);

	unsigned int indexCount = (ui_rows - 1) * (ui_cols - 1) * 6;

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}



