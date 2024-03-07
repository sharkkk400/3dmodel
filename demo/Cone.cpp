#include "Cone.h"

Cone::Cone(float r, float h, int x)
{
	//Ô²×¶(¹² x+2 ¸öµã)
	for (int i = 0; i < x; i++) {
		Vertex.push_back(QVector3D(r*cos(i * 2 * PI / float(x)), -h / 2.0, -r * sin(i * 2 * PI / float(x))));
	}
	Vertex.push_back(QVector3D(0, -h / 2.0, 0));
	Vertex.push_back(QVector3D(0, h / 2.0, 0));

	for (int i = 0; i < x; i++) {
		index.push_back(x);
		index.push_back(i);
		index.push_back(i + 1 < x ? i + 1 : 0);
		index.push_back(x + 1);
		index.push_back(i + 1 < x ? i + 1 : 0);
		index.push_back(i);
	}
}

Cone::~Cone()
{
}
