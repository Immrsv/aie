#include "Scene.h"

#include <glm/glm.hpp>
//#include <glm/ext.hpp>
#include <gl_core_4_4.h>
//#include <stb_image.h>
#include <FBXFile.h>
#include <string>

Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::init() {

	camera.setViewFor(vec3(0, 10, -10), 90.f, -45.f);

	// Load Shaders
	//Shader::CompileShaders("DefaultShader", "../Project3D/Basic.vert", "../Project3D/Basic.frag");
	//Shader::CompileShaders("LightedShader", "../Project3D/Lighted.vert", "../Project3D/Lighted.frag");
	//Shader::CompileShaders("TexturedShader", "../Project3D/Textured.vert", "../Project3D/Textured.frag");
	Shader::CompileShaders("MorphingShader", "../Project3D/Morphing.vert", "../Project3D/Morphing.frag");

	// Load Textures
	m_textures.push_back(new aie::Texture("./textures/numbered_grid.tga"));

	// Load Models
	//m_entities.push_back(
	//		new Entity(Model::LoadModel("./models/stanford/bunny.obj"), 
	//		new aie::Texture("./textures/numbered_grid.tga"),
	//		Shader::GetProgramID("TexturedShader"))
	//);
	
	//m_entities.push_back(new Entity(Model::LoadModel("./models/Robot/mecanimloco.fbx"), new aie::Texture("./textures/numbered_grid.tga"), Shader::GetProgramID("TexturedShader")));

	Entity* e = new Entity(Model::LoadModel("./models/hand/hand_00.obj", "./models/hand/hand_36.obj"), new aie::Texture("./models/hand/hand.png"), Shader::GetProgramID("MorphingShader"));
	

	m_entities.push_back(e);

	//m_entities[1]->m_transform = glm::translate(vec3(-10, 0, 0));
	//m_entities[2]->m_transform = glm::translate(vec3(10, 0, 0));
	//m_entities[3]->m_transform = glm::translate(vec3(-10, 0, -10));


	// Setup Lighting

}


void Scene::update(float deltaTime) {

	for (auto entity : m_entities) {
		entity->timestep += deltaTime * 1.f;

	}

	camera.update(deltaTime);
}

void Scene::draw() {
	
	for (auto entity : m_entities) {
		glUseProgram(entity->ui_shaderID);

		unsigned int pvmUniform = glGetUniformLocation(entity->ui_shaderID, "pvmMatrix");
		unsigned int modelUniform = glGetUniformLocation(entity->ui_shaderID, "modelMatrix");
		GLuint cameraPosUniform = glGetUniformLocation(entity->ui_shaderID, "cameraPos");

		// set texture slot 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity->m_texture->getHandle());

		GLuint loc;
		// tell the shader where it is 
		loc = glGetUniformLocation(entity->ui_shaderID, "diffuseTex"); 
		glUniform1i(loc, /*GL_TEXTURE*/ 0);

		loc = glGetUniformLocation(entity->ui_shaderID, "keyTime");
		glUniform1f(loc, sin(entity->timestep) * 0.5f + 0.5f );

		glUniformMatrix4fv(pvmUniform, 1, false, glm::value_ptr(camera.getTransform() * entity->m_transform));
		glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(entity->m_transform));

		//glUniform3fv(cameraPosUniform, 3, glm::value_ptr(camera.getPosition()));
		glUniform3f(cameraPosUniform, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		// example cosine interpolation between [0,1] 
		//int loc = glGetUniformLocation(entity->ui_shaderID, "keyTime");
		//glUniform1f(loc, cosf(entity->timestep) * 0.5f + 0.5f);

		entity->m_model->draw();
	}
}

