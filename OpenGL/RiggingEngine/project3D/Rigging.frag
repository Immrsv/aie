#version 410

in vec4 vNormal;
in vec4 vPosition;
in vec2 uv;

in vec3 debug;

out vec4 fragColor;

vec3 ambient = vec3(0.1,0.1,0.1);
vec3 diffuse = vec3(0);
vec3 specular = vec3(0);
float alpha = 1.0;

uniform vec4 lightDir = vec4(0.7,0.7,0.,0.);
uniform vec3 lightClr = vec3(1.,0.5,0.);

uniform vec3 cameraPos = vec3(0);
uniform float specPow = 20.0;

uniform sampler2D diffuseTex;

void main() { 

	//fragColor = vColour;
	//fragColor = vec4(0.5);

	
	diffuse = lightClr * clamp(dot(vNormal, lightDir), 0., 1.);
	 

	//vec3 E = normalize( cameraPos - vPosition.xyz).xyz; // Direction to Camera from vPosition
	//vec3 R = reflect( -lightDir.xyz, vNormal.xyz).xyz; // Reflected Ray
	//float s = max( 0, dot( E, R ));	// Specular value
	//s = pow( s, specPow ); // Raise to Specular Power

	//specular = lightClr * s;

	//// Apply Texture instead
	//diffuse = texture(diffuseTex, uv).xyz;

	fragColor = clamp(vec4( ambient + diffuse + specular, alpha), 0, 1);
	//fragColor = vec4(debug, 1);
}