// VERTEX SHADER 
#version 410 

layout(location=0) in vec4 position;
layout(location=1) in vec4 normal;
layout(location=2) in vec4 tangent;
layout(location=3) in vec2 texcoord;
layout(location=4) in vec4 weights;
layout(location=5) in vec4 indices;

out vec3 vNormal;
out vec3 vPosition;
out vec2 uv;

out vec3 debug;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;

const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main() {

	ivec4 index = ivec4(indices);

	vec4 P = bones[ index.x ] * position * weights.x;
	P += bones[ index.y ] * position * weights.y;
	P += bones[ index.z ] * position * weights.z;
	P += bones[ index.w ] * position * weights.w;

    vPosition = (modelMatrix * vec4(position.xyz,1)).xyz;
    vNormal = (modelMatrix * vec4(normal.xyz,0)).xyz;
    uv = texcoord;

	debug = (bones[ index.y ] * position * weights.y).xyz;

    gl_Position = pvmMatrix * vec4(P.xyz, 1);
}
