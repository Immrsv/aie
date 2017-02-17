// VERTEX SHADER 
#version 410 

layout( location = 0 ) in vec3 Position; 
layout( location = 1 ) in vec3 Normal; 
layout( location = 2 ) in vec2 TexCoord; 

// from VBO 2 
layout( location = 3 ) in vec3 Position2; 
layout( location = 4 ) in vec3 Normal2; 

out vec3 vNormal; 
out vec3 vPosition;
out vec2 uv; 

uniform mat4 pvmMatrix; 
uniform mat4 modelMatrix;

// interpolation value between [0,1] 
uniform float keyTime; 

void main() { 
	// lerp position and normals 
	vec3 p = mix(Position, Position2, keyTime); 
	vec3 n = mix(Normal, Normal2, keyTime); 

	vPosition = (modelMatrix * vec4(p,1)).xyz;
	vNormal = (modelMatrix * vec4(n,0)).xyz; 
	uv = TexCoord; 

	gl_Position = pvmMatrix * vec4(p, 1); 
}