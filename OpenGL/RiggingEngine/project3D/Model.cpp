#include "Model.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <iostream>


using glm::vec3;
using glm::vec4;
using glm::mat4;

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
	model->loadFbx(file.c_str());
	m_models[file] = model;

	std::cout << " - DONE!" << std::endl;
	

	return model;
	
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

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normals
		glEnableVertexAttribArray(2); //tangents
		glEnableVertexAttribArray(3); //texcoords
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices 

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)FBXVertex::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)FBXVertex::IndicesOffset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = glData;

	}
}


void Model::draw() {

	// Use "FBX" data
	// bind our vertex array object and draw the mesh 
	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i) {

		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;

		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}	
}
