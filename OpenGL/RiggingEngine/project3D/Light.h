#pragma once

#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;

class Light
{
public:
	Light();
	~Light();

	vec4 v4_color;

	vec3 v3_direction;
};

