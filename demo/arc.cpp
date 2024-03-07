#include "arc.h"

arc::arc(float r, int x)
{
	drawitem = DRAW_LINE;

	for (int i = 0; i < x; i++) {
		Vertex.push_back(QVector3D(r*cos(i * 2 * PI / float(x)), 0 , -r * sin(i * 2 * PI / float(x))));
	}

	for (int i = 0; i < x; i++) {
		index.push_back(i);
		index.push_back(i + 1 < x ? i + 1 : 0);
	}

}

arc::~arc()
{
}
