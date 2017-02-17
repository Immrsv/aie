#pragma once
#include "Light.h"

class PointLight : Light
{
public:
	PointLight();
	~PointLight();

	vec3 v3_position;
};

