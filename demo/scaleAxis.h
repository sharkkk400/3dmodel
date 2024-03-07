#pragma once
#include "transAxis.h"
class scaleAxis : public transAxis
{
public:
	scaleAxis();
	~scaleAxis();

	cylinder Xcylinder;
	cube Xcube;
	cylinder Ycylinder;
	cube Ycube;
	cylinder Zcylinder;
	cube Zcube;

	square XOY;
	square XOZ;
	square YOZ;

	sphere XYZ;

};

