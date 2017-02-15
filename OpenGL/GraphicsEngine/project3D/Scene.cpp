#include "Scene.h"

#include <glm/glm.hpp>
//#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <stb_image.h>


Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::init() {

	camera.setViewFor(vec3(0, 10, -10), 90.f, -45.f);

	// Load Shaders
	Shader::CompileShaders("DefaultShader", "../Project3D/Basic.vert", "../Project3D/Basic.frag");
	Shader::CompileShaders("LightedShader", "../Project3D/Lighted.vert", "../Project3D/Lighted.frag");
	Shader::CompileShaders("TexturedShader", "../Project3D/Textured.vert", "../Project3D/Textured.frag");

	// Load Textures


	// Load Models
	m_entities.push_back(new Entity(Model::LoadModel("./models/stanford/bunny.obj"), Shader::GetProgramID("TexturedShader")));
	//m_entities.push_back(new Entity(Model::LoadModel("./models/stanford/bunny.obj"), Shader::GetProgramID("DefaultShader")));
	//m_entities.push_back(new Entity(Model::LoadModel("./models/stanford/bunny.obj"), Shader::GetProgramID("DefaultShader")));
	//m_entities.push_back(new Entity(Model::LoadModel("./models/stanford/dragon.obj"), Shader::GetProgramID("DefaultShader")));

	//m_entities[1]->m_transform = glm::translate(vec3(-10, 0, 0));
	//m_entities[2]->m_transform = glm::translate(vec3(10, 0, 0));
	//m_entities[3]->m_transform = glm::translate(vec3(-10, 0, -10));


	// Setup Lighting
}

void Scene::update(float deltaTime) {

	camera.update(deltaTime);
}

void Scene::draw() {
	
	for (auto entity : m_entities) {
		glUseProgram(entity->ui_shaderID);

		unsigned int pvmUniform = glGetUniformLocation(entity->ui_shaderID, "pvmMatrix");
		unsigned int modelUniform = glGetUniformLocation(entity->ui_shaderID, "modelMatrix");
		GLuint cameraPosUniform = glGetUniformLocation(entity->ui_shaderID, "cameraPos");

		static bool runOnce = true;
		if (runOnce) {
			runOnce = cameraPosUniform == 0;
			std::cout << "cameraPosUniform: " << cameraPosUniform << std::endl;
		}


		glUniformMatrix4fv(pvmUniform, 1, false, glm::value_ptr(camera.getTransform() * entity->m_transform));
		glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(entity->m_transform));

		//glUniform3fv(cameraPosUniform, 3, glm::value_ptr(camera.getPosition()));
		glUniform3f(cameraPosUniform, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		entity->m_model->draw();
	}
}