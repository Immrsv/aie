#pragma once

#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Model.h"
#include <glm/mat4x4.hpp>

class Entity {
public:
	Entity(Model* model, unsigned int shaderID) : m_model(model), ui_shaderID(shaderID) {}
	
	Model* m_model;
	unsigned int ui_shaderID;
	glm::mat4 m_transform;
};

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	Camera		camera;

	unsigned int defaultShader;
	std::vector<Entity*> m_entities;
};