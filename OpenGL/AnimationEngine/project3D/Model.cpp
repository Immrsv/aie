#include "Model.h"

#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;

// static
std::map<std::string, Model*> Model::m_models = std::map<std::string, Model*>();

Model* Model::LoadModel(std::string file)
{

	std::cout << "Loading Model: " << file << std::flush;

	std::map<std::string, Model*>::iterator iter = m_models.find(file);
	if (iter != m_models.end()) {

		std::cout << " - CACHED!" << std::endl;
		return iter->second;
	}

	Model* model = new Model();
	model->load(file.c_str());

	std::cout << " - DONE!" << std::endl;

	m_models[file] = model;

	return model;
}

Model* Model::LoadFbxModel(std::string file) {
	std::cout << "Loading FBX Model: " << file << std::flush;

	std::map<std::string, Model*>::iterator iter = m_models.find(file);
	if (iter != m_models.end()) {

		std::cout << " - CACHED!" << std::endl;
		return iter->second;
	}

	Model* model = new Model();
	model->loadFbx(file.c_str());

	std::cout << " - DONE!" << std::endl;

	m_models[file] = model;

	return model;
}

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


	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, file);


	createGLBuffers(attribs, shapes);
}

void Model::loadFbx(const char* file) {
	throw "Model::loadFbx(filename) Not Implemented!";
}

void Model::draw() {

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
		
		//position 
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0); 
				
		//normal data 
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12); 
				
		//texture data 
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24); 


		glBindVertexArray(0); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		
		shapeIndex++;
	}
}
