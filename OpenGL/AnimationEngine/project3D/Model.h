#pragma once


#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <vector>
#include <FBXFile.h>

class Model
{
public:
	Model();
	~Model();
	
	void draw();

	static Model* LoadModel(std::string file, std::string file2);

private:


	static std::map<std::string, Model*> m_models;

	struct OBJVertex { float x, y, z; float nx, ny, nz; float u, v; };

	struct OpenGLInfo {
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_VBO2;
		unsigned int m_faceCount;
	};

	std::vector<OpenGLInfo> m_glInfo;

	FBXFile* m_fbx;

	void createGLBuffers(tinyobj::attrib_t& attribs, std::vector<tinyobj::shape_t>& shapes);
	unsigned int createVertexBuffer(const tinyobj::attrib_t& attribs, const tinyobj::shape_t& shape);

	void load(const char* file, const char* file2);
	void loadFbx(const char* file);
};

