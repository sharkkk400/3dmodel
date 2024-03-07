#pragma once
#include "transAxis.h"
class moveAxis : public transAxis
{
public:

	moveAxis();
	~moveAxis();

	cylinder Xcylinder;
	Cone Xcone;
	cylinder Ycylinder;
	Cone Ycone;
	cylinder Zcylinder;
	Cone Zcone;

	square XOY;
	square XOZ;
	square YOZ;

	sphere XYZ;

	bool checkin(QVector2D viewXY);

	void unifycylinderandcone();
	
};

