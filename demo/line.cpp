#include "line.h"

line::line(float len)
{
	drawitem = DRAW_LINE;

	Vertex.push_back(QVector3D(len/2.0, 0, 0));
	Vertex.push_back(QVector3D(-len / 2.0, 0, 0));

	index.push_back(0);
	index.push_back(1);
}

line::~line()
{
}
