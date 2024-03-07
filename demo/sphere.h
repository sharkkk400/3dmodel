#pragma once
#include "headers.h"
#include "meshComponent.h"

class sphere : public meshComponent
{
public:
	sphere(float r = 0.25, int x = 20);
	~sphere();
};
