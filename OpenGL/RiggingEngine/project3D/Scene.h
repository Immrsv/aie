#pragma once


#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include <Texture.h>

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include <Texture.h>
#include <FBXFile.h>
#include <vector>

using glm::mat4;
using glm::vec3;

struct Entity {
public:
	Entity(Model* model, aie::Texture* texture, unsigned int shaderID) : m_model(model), m_texture(texture), ui_shaderID(shaderID) {}

	Model* m_model;
	unsigned int ui_shaderID;
	glm::mat4 m_transform;
	aie::Texture* m_texture;
	float timestep = 0.0f;

};


class Scene
{
public:
	Scene();
	~Scene();

	void init();
	void update(float deltaTime);
	void draw();

	Camera camera;	
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	glm::vec3 v3_ambientLight;
	
	unsigned int defaultShader;

	std::vector<Entity*> m_entities;
	std::vector<Light*> m_lights;
	std::vector<aie::Texture*> m_textures;

};

