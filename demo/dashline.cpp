#include "dashline.h"

dashline::dashline(float len)
{
	drawitem = DRAW_LINE;

	float lenth = len/30.0;

	for (int i = 0; i < 60; i++) {
		Vertex.push_back(QVector3D(0, 0, 0) + i * QVector3D(lenth, 0, 0));
		index.push_back(i);
	}
}

dashline::~dashline()
{
}
