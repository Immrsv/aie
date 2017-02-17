#version 410

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec2 uvs;

out vec4 vNormal;
out vec4 vPosition;
out vec2 uv;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;

void main() {
	vNormal = modelMatrix * normal; 
	vPosition = modelMatrix * position;

	uv = uvs;

	gl_Position = pvmMatrix * position; 
}
