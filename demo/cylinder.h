#pragma once
#include "meshComponent.h"
class cylinder : public meshComponent
{
public:
	cylinder(float len = 0.5, float r = 0.1, int x = 15);
	~cylinder();

};

