#pragma once
#include "transAxis.h"

class rotateAxis : public transAxis
{
public:
	rotateAxis();
	~rotateAxis();

	
	arc X;
	arc Y;
	arc Z;

};

