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

	vec4 tempWeight;// =  weights * vec4( 1.f,1.f,1.f,1.f );
	tempWeight = normalize(weights);
	
	vec3 P = (bones[ index.x ] * position * weights.x).xyz;
		P += (bones[ index.y ] * position * weights.y).xyz;
		P += (bones[ index.z ] * position * weights.z).xyz;
		P += (bones[ index.w ] * position * weights.w).xyz;

	vec4 N = inverse(transpose(bones[ index.x ])) * normal * tempWeight.x;
	N += inverse(transpose(bones[ index.y ])) * normal * tempWeight.y;
	N += inverse(transpose(bones[ index.z ])) * normal * tempWeight.z;
	N += inverse(transpose(bones[ index.w ])) * normal * tempWeight.w;

    vPosition = (modelMatrix * vec4(P.xyz,1)).xyz;
    vNormal = normalize((modelMatrix * vec4(N.xyz,0)).xyz);
    uv = texcoord;

	//debug = vec3(1);//normalize(tempWeight.xyz);

    gl_Position = pvmMatrix * vec4(P.xyz, 1);
}
