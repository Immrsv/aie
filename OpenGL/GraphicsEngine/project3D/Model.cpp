#include "Model.h"

#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <iostream>

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

	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	std::cout << "Loading Model: " << file << std::flush;

	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, file);

	std::cout << " - DONE!" << std::endl;

	createGLBuffers(attribs, shapes);
}

void Model::generatePlane() {
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

	for (auto& gl : m_glInfo) { 
		glBindVertexArray(gl.m_VAO); 
		glDrawArrays(GL_TRIANGLES, 0, gl.m_faceCount * 3); 
	}
}

void Model::createGLBuffers(tinyobj::attrib_t & attribs, std::vector<tinyobj::shape_t>& shapes)
{
	m_glInfo.resize(shapes.size());

	struct OBJVertex { float x, y, z; float nx, ny, nz; float u, v; };

	std::vector<OBJVertex> vertices;

	int shapeIndex = 0;

	for (auto& shape : shapes) {
	
		// Generate GL Buffers
		glGenVertexArrays(1, &m_glInfo[shapeIndex].m_VAO);
		glGenBuffers(1, &m_glInfo[shapeIndex].m_VBO);
		glBindVertexArray(m_glInfo[shapeIndex].m_VAO);
		m_glInfo[shapeIndex].m_faceCount = shape.mesh.num_face_vertices.size();


		// Collect triangles
		std::vector<OBJVertex> vertices;
		int index = 0;
		for (auto face : shape.mesh.num_face_vertices) {
			for (int i = 0; i < 3; ++i) {
				tinyobj::index_t idx = shape.mesh.indices[index + i]; 
				OBJVertex v = { 0 }; 
				// positions 
				v.x = attribs.vertices[3 * idx.vertex_index + 0]; 
				v.y = attribs.vertices[3 * idx.vertex_index + 1]; 
				v.z = attribs.vertices[3 * idx.vertex_index + 2]; 
				
				// normals 
				if (attribs.normals.size() > 0) { 
					v.nx = attribs.normals[3 *idx.normal_index + 0]; 
					v.ny = attribs.normals[3 *idx.normal_index + 1]; 
					v.nz = attribs.normals[3 *idx.normal_index + 2]; 
				} 
				// texture coordinates 
				if (attribs.texcoords.size() > 0) { 
					v.u = attribs.texcoords[2*idx.texcoord_index+0]; 
					v.v = attribs.texcoords[2*idx.texcoord_index+1]; 
				} 
				
				vertices.push_back(v);
			}
			index += face;
		}

		// bind vertex data 
		glBindBuffer(GL_ARRAY_BUFFER, m_glInfo[shapeIndex].m_VBO); 
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW); 
		glEnableVertexAttribArray(0); 
		
		//position 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0); 
		glEnableVertexAttribArray(1); 
		
		//normal data 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12); 
		glEnableVertexAttribArray(2); 
		
		//texture data 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24); 
		glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER, 0); 
		
		shapeIndex++;
	}
}
