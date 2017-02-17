#version 410

in vec4 vNormal;
in vec4 vPosition;
in vec2 uv;

out vec4 fragColor;

vec3 ambient = vec3(0.1,0.1,0.1);

uniform vec4 lightDir = vec4(0.7,0.7,0,0);
uniform vec3 lightClr = vec3(1,0.5,0);

uniform vec3 cameraPos = vec3(0);
uniform float specPow = 10.0;

uniform sampler2D diffuseTex;

void main() { 

	//fragColor = vColour;
	//fragColor = vec4(0.5);

	float alpha = 1.0;
	vec3 diffuse = lightClr * clamp(dot(vNormal, lightDir), 0., 1.);
	 

	vec3 E = normalize( cameraPos - vPosition.xyz); // Direction to Camera from vPosition
	vec3 R = reflect( -lightDir.xyz, vNormal.xyz); // Reflected Ray
	float s = max( 0, dot( E, R ));	// Specular value
	s = pow( s, specPow ); // Raise to Specular Power

	vec3 specular = lightClr * s;

	// Apply Texture instead
	diffuse = texture(diffuseTex, vPosition.xy).xyz;

	fragColor = clamp(vec4( ambient + diffuse + specular, alpha), 0, 1);
}