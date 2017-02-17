#pragma once
#include "PointLight.h"

class SpotLight :
	public PointLight
{
public:
	SpotLight();
	~SpotLight();

	unsigned int ui_fov;
	unsigned int ui_falloff;

};

