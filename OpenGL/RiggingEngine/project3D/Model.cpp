#include "Model.h"

#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <iostream>


using glm::vec3;
using glm::vec4;
using glm::mat4;

// static
std::map<std::string, Model*> Model::m_models = std::map<std::string, Model*>();

Model* Model::LoadModel(std::string file, std::string file2)
{

	std::cout << "Loading Model: " << file << std::flush;

	std::map<std::string, Model*>::iterator iter = m_models.find(file);
	if (iter != m_models.end()) {

		std::cout << " - CACHED!" << std::endl;
		return iter->second;
	}

	Model* model = 0;

	if (file.find(".obj") != std::string::npos) {
		model = new Model();
		model->load(file.c_str(), file2.c_str());
	}
	if (file.find(".fbx") != std::string::npos) {
		model = new Model();
		model->loadFbx(file.c_str());
	}

	if (model != 0) {
		std::cout << " - DONE!" << std::endl;

		m_models[file] = model;

		return model;
	}

	std::cout << " - Error: Unknown Format!" << std::endl;
	return 0;
}



struct Vertex {
	glm::vec4 position;
	glm::vec4 color;
};


Model::Model() : m_fbx(0)
{
}


Model::~Model()
{
}

void Model::load(const char* file, const char* file2) {

	tinyobj::attrib_t attribs[2];
	std::vector<tinyobj::shape_t> shapes[2];
	std::vector<tinyobj::material_t> materials[2];
	std::string err;


	bool success = tinyobj::LoadObj(&attribs[0], &shapes[0], &materials[0], &err, file);
	success &= tinyobj::LoadObj(&attribs[1], &shapes[1], &materials[1], &err, file2);


	// loaded models have matching shape counts 
	for (int i = 0; i < shapes[0].size(); ++i) {
		OpenGLInfo gl;

		gl.m_VBO = createVertexBuffer(attribs[0], shapes[0][i]);
		gl.m_VBO2 = createVertexBuffer(attribs[1], shapes[1][i]);

		// shapes have matching face counts 
		gl.m_faceCount = shapes[0][i].mesh.num_face_vertices.size();
		glGenVertexArrays(1, &gl.m_VAO);
		glBindVertexArray(gl.m_VAO);

		// bind first VBO 
		glBindBuffer(GL_ARRAY_BUFFER, gl.m_VBO);
		glEnableVertexAttribArray(0);

		// position 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0);
		glEnableVertexAttribArray(1);
		// normal data 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12); 
		glEnableVertexAttribArray(2);

		// texture data 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24);

		// bind second VBO 
		glBindBuffer(GL_ARRAY_BUFFER, gl.m_VBO2);
		glEnableVertexAttribArray(3);

		// position 2 
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0); 
		glEnableVertexAttribArray(4);
		// normal 2 
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12);
		glBindVertexArray(0);

		m_glInfo.push_back(gl);
	}
}

void Model::loadFbx(const char * file)
{
	m_fbx = new FBXFile();
	m_fbx->load(file);
	
	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {

		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);

		// allocate storage for gl Indeces
		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);

		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);

		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position 
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1); // normal 
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;

	}
}


void Model::draw() {

	if (m_fbx == 0) { // Use "OBJ" data
		for (auto& gl : m_glInfo) {
			glBindVertexArray(gl.m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, gl.m_faceCount * 3);
		}
	}
	else { // Use "FBX" data
		// bind our vertex array object and draw the mesh 
		for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {

			FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
			unsigned int* glData = (unsigned int*)mesh->m_userData;

			glBindVertexArray(glData[0]);
			glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
}

void Model::createGLBuffers(tinyobj::attrib_t & attribs, std::vector<tinyobj::shape_t>& shapes)
{
	throw "Model::createGLBuffers - Not Implemented!";
	//m_glInfo.resize(shapes.size());

	//int shapeIndex = 0;
	//
	//for (auto& shape : shapes) {
	//
	//	// Generate GL Buffers
	//	glGenVertexArrays(1, &m_glInfo[shapeIndex].m_VAO);
	//	glBindVertexArray(m_glInfo[shapeIndex].m_VAO);

	//	m_glInfo[shapeIndex].m_faceCount = shape.mesh.num_face_vertices.size();

	//	m_glInfo[shapeIndex].m_VBO = createVertexBuffer(attribs[0], shapes[0][i]);


	//	// bind vertex data 
	//	glBindBuffer(GL_ARRAY_BUFFER, m_glInfo[shapeIndex].m_VBO); 
	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW); 
	//	
	//	//position 
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0); 
	//			
	//	//normal data 
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12); 
	//			
	//	//texture data 
	//	glEnableVertexAttribArray(2);
	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24); 


	//	glBindVertexArray(0); 
		
	//	shapeIndex++;
	//}
}

unsigned int Model::createVertexBuffer(const tinyobj::attrib_t & attribs, const tinyobj::shape_t & shape)
{
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
				v.nx = attribs.normals[3 * idx.normal_index + 0];
				v.ny = attribs.normals[3 * idx.normal_index + 1];
				v.nz = attribs.normals[3 * idx.normal_index + 2];
			}
			// texture coordinates 
			if (attribs.texcoords.size() > 0) {
				v.u = attribs.texcoords[2 * idx.texcoord_index + 0];
				v.v = attribs.texcoords[2 * idx.texcoord_index + 1];
			}

			vertices.push_back(v);
		}
		index += face;
	}

	// bind vertex data 
	unsigned int buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return buffer;
}
