#pragma once

#include <map>
#include <FBXFile.h>

class Model
{
public:
	Model();
	~Model();
	
	void draw();

	static Model* LoadModel(std::string file);

	FBXFile* m_fbx;
private:


	static std::map<std::string, Model*> m_models;

	void loadFbx(const char* file);
};

