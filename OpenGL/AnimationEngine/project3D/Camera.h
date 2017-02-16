#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec2;

class Camera
{
public:
	Camera();
	~Camera();

	
	float fov;

	virtual void update(float deltaTime);

	//void lookAt(vec3 from, vec3 to, vec3 up);
	void translate(vec3 delta);
	void translateWorld(vec3 worldDelta);

	void setViewFor(vec3 position, float theta, float phi);
	void setPosition(vec3 position);
	vec3 getPosition();

	void setTheta(float theta);
	void setPhi(float phi);
	float getTheta();
	void setProjection(mat4 projection);
	mat4 getView();
	mat4 getProjection();
	mat4 getTransform();

private:
	bool isViewDirty;
	bool isTransformDirty;

	float f_theta;
	float f_phi;

	vec3 v3_position;
	mat4 m4_view;
	mat4 m4_projectedView;

	mat4 m4_projection;
};

