#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <FBXFile.h>

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
	//Shader::CompileShaders("MorphingShader", "../Project3D/Morphing.vert", "../Project3D/Morphing.frag");
	Shader::CompileShaders("RiggingShader", "../Project3D/Rigging.vert", "../Project3D/Rigging.frag");

	// Load Textures
	m_textures.push_back(new aie::Texture("./textures/numbered_grid.tga"));

	// Load Models
	//m_entities.push_back(new Entity(Model::LoadModel("./models/Robot/mecanimloco.fbx"), new aie::Texture("./textures/numbered_grid.tga"), Shader::GetProgramID("RiggingShader")));
	m_entities.push_back(new Entity(Model::LoadModel("./models/Pyro/pyro.fbx"), new aie::Texture("./textures/numbered_grid.tga"), Shader::GetProgramID("RiggingShader")));
	m_entities.back()->m_transform = glm::rotate(3.f, glm::vec3(0, 1, 0));

	// Setup Lighting

}


void Scene::update(float deltaTime) {

	for (auto entity : m_entities) {
		entity->timestep += deltaTime * 1.f;

		FBXSkeleton* skel = entity->m_model->m_fbx->getSkeletonByIndex(0);
		FBXAnimation* anim = entity->m_model->m_fbx->getAnimationByIndex(0);

		//if (entity->timestep > anim->totalTime()) 
		//	entity->timestep = 0.f;

		skel->evaluate(anim, entity->timestep);

		for (unsigned int bone_index = 0; bone_index < skel->m_boneCount; ++bone_index)
		{
			skel->m_nodes[bone_index]->updateGlobalTransform();
		}

	}

	camera.update(deltaTime);
}

void Scene::draw() {
	
	for (auto entity : m_entities) {
		FBXSkeleton* skeleton = entity->m_model->m_fbx->getSkeletonByIndex(0);
		skeleton->updateBones();

		glUseProgram(entity->ui_shaderID);

		unsigned int pvmUniform = glGetUniformLocation(entity->ui_shaderID, "pvmMatrix");
		unsigned int modelUniform = glGetUniformLocation(entity->ui_shaderID, "modelMatrix");
		unsigned int bonesUniform = glGetUniformLocation(entity->ui_shaderID, "bones");
		GLuint cameraPosUniform = glGetUniformLocation(entity->ui_shaderID, "cameraPos");

		// set texture slot 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity->m_texture->getHandle());

		glm::mat4 scaledModel = glm::scale(entity->m_transform, vec3(0.01f));

		GLuint loc;
		// tell the shader where it is 
		loc = glGetUniformLocation(entity->ui_shaderID, "diffuseTex"); 
		glUniform1i(loc, /*GL_TEXTURE*/ 0);

		glUniformMatrix4fv(pvmUniform, 1, false, glm::value_ptr(camera.getTransform() * scaledModel));
		glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(scaledModel));
		glUniformMatrix4fv(bonesUniform, skeleton->m_boneCount, false, (float*)skeleton->m_bones);

		glUniform3fv(cameraPosUniform, 1, glm::value_ptr(camera.getPosition()));
		//glUniform3f(cameraPosUniform, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		entity->m_model->draw();
	}
}

