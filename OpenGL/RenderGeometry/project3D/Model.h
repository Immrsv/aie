#pragma once

#include <gl_core_4_4.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec4;
using glm::vec3;

struct Vertex {
	vec4 position;
	vec4 color;
};

class Model
{
	
public:
	Model();
	~Model();

	unsigned int ui_rows, ui_cols;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_programID;

	void init() {
		// create shaders 
		const char* vsSource = "#version 410\n \
			layout(location=0) in vec4 position; \
 layout(location=1) in vec4 colour; \
 out vec4 vColour; \
 uniform mat4 projectionViewWorldMatrix; \
 void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position; }";

		const char* fsSource = "#version 410\n \
 in vec4 vColour; \
 out vec4 fragColor; \
 void main() { fragColor = vColour; }";

		int success = GL_FALSE;

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(fragmentShader);

		m_programID = glCreateProgram();
		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);
		glLinkProgram(m_programID);
		glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

		if (success == GL_FALSE) {
			int infoLogLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);

			printf("Error: Failed to link shader program!\n");
			printf("%s\n", infoLog); delete[] infoLog;
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}

	void generateGrid(unsigned int rows, unsigned int cols) {

		ui_rows = rows;
		ui_cols = cols;

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

	void draw(glm::mat4 projectionViewMatrix) {

		glUseProgram(m_programID); 

		unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix"); 

		glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));
		glBindVertexArray(m_VAO); 
		
		unsigned int indexCount = (ui_rows - 1) * (ui_cols - 1) * 6;

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
};

