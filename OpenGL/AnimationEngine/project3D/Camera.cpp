#include "Camera.h"
#include "Input.h"
#include <iostream>

const float deg2rad = glm::pi<float>() / 180.f;

Camera::Camera() : 
	isViewDirty(true), 
	isTransformDirty(false), 
	m4_projection(1), 
	m4_view(1), 
	v3_position(0), 
	f_theta(0.f), 
	f_phi(0.f)
{
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime) {

	//return;

	static aie::Input* input = aie::Input::getInstance();

	// Capture Motion Input
	vec3 move = vec3(0.f);
	float scaleTranslation = 4.0;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		scaleTranslation *= 2.0;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		move += vec3(0, 0, -1) * deltaTime * scaleTranslation; // Forward
	if (input->isKeyDown(aie::INPUT_KEY_S))
		move += vec3(0, 0, 1) * deltaTime * scaleTranslation;  // Back
	if (input->isKeyDown(aie::INPUT_KEY_A))
		move += vec3(-1, 0, 0) * deltaTime * scaleTranslation; // Left
	if (input->isKeyDown(aie::INPUT_KEY_D))
		move += vec3(1, 0, 0) * deltaTime * scaleTranslation; // Right
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
		move += vec3(0, 1, 0) * deltaTime * scaleTranslation; // Up
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL))
		move += vec3(0, -1, 0) * deltaTime * scaleTranslation;  // Down

	if (move.length() > 0.00000001) { // Epsilon
		//setPosition(v3_position + move);
		translate(move);
	}

	static vec2 lastMouse;
	static bool isMouseTracking = false;

	vec2 mouse = vec2(input->getMouseX(), input->getMouseY());

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT)) {
		
		const float scaleRotation = 0.05f;
		vec2 deltaMouse = (mouse - lastMouse) * scaleRotation;

		setTheta(f_theta + deltaMouse.x);
		setPhi(f_phi + deltaMouse.y);

	}
	lastMouse = mouse;

	//std::cout << "Position: " << v3_position.x << ", " << v3_position.y << ", " << v3_position.z << std::endl;

}

void Camera::translate(vec3 delta)
{
	// Convert to "camera" space
	delta = vec3(glm::inverse(getView()) * glm::vec4(delta, 0));

	v3_position += delta;
	isViewDirty = true;
}

void Camera::translateWorld(vec3 worldDelta)
{
	v3_position += worldDelta;
	isViewDirty = true;
}


void Camera::setViewFor(vec3 position, float theta, float phi) {
	setPosition(position);
	setTheta(theta);
	setPhi(phi);
}

void Camera::setPosition(vec3 position) {
	v3_position = position;
	isViewDirty = true;
}
vec3 Camera::getPosition()
{
	return v3_position;
}

void Camera::setTheta(float theta) {
	f_theta = theta;
	isViewDirty = true;
}

float Camera::getTheta() {
	return f_theta;
}

void Camera::setPhi(float phi) {
	f_phi = phi;
	isViewDirty = true;
}

mat4 Camera::getView() {
	if (isViewDirty) {

		float thetaR = f_theta * deg2rad;
		float phiR = f_phi * deg2rad;

		vec3 forward(cos(phiR)*cos(thetaR), sin(phiR), cos(phiR)*sin(thetaR));
		m4_view = glm::lookAt(v3_position, v3_position + forward, vec3(0, 1, 0));

		isViewDirty = false;
		isTransformDirty = true;
	}

	return m4_view;
}

mat4 Camera::getProjection() {
	return m4_projection;
}

void Camera::setProjection(mat4 projection)
{
	m4_projection = projection;
	isTransformDirty = true;
}

mat4 Camera::getTransform()
{
	if (isViewDirty || isTransformDirty)
		m4_projectedView = getProjection() * getView();
	
	isTransformDirty = false;
	return m4_projectedView;
}
