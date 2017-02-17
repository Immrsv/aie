#pragma once


#include <glm/mat4x4.hpp>

#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Model.h"
#include "Scene.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:



	Scene scene;

};