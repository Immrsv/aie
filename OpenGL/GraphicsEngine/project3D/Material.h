#pragma once

#include <glm/glm.hpp>

using glm::vec4;

class Material
{
public:
	Material();
	~Material();

	vec4 v4_ambientColor;
	vec4 v4_diffuseColor;
	vec4 v4_specularColor;

	unsigned int ui_specularPower;

};

