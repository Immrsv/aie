#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;

out vec4 vNormal;
out vec4 vPosition;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;

void main() {
	vNormal = modelMatrix * normal; 
	vPosition = modelMatrix * position;
	gl_Position = pvmMatrix * position; 
}
