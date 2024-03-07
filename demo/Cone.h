#pragma once
#include "meshComponent.h"
class Cone : public meshComponent
{
public:
	Cone(float r = 1.0 , float h = 2.0 , int x = 15);
	~Cone();
};

