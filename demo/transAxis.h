#pragma once
#include "arc.h"
#include "Cone.h"
#include "cube.h"
#include "cylinder.h"
#include "sphere.h"
#include "square.h"
class transAxis
{
public:
	transAxis();
	~transAxis();

	bool isDraw;
	QMatrix4x4 base;

	std::vector<meshComponent*> components;

	int item;
	std::map<int, int> unititem;

	void init();
	void draw();

	void setdefaultStatus();
	void setcheckeditem(int item);
	bool checkinitem(QVector2D viewXY);
};

