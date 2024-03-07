#pragma once
#include "meshComponent.h"
class cube :
	public meshComponent
{
public:
	cube(float x = 0.5 , float y = 0.5, float z = 0.5);
	~cube();
};

