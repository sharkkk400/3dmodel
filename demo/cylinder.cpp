#include "cylinder.h"

cylinder::cylinder(float len, float r,int x)
{
	//Ô²Öù£¨¹² 2x+2 ¸öµã£©
	for (int i = 0; i < x; i++) {
		Vertex.push_back(QVector3D(r *cos(i * 2 * PI / float(x)), -len / 2.0, -r *sin(i * 2 * PI / float(x))));
		Vertex.push_back(QVector3D(r *cos(i * 2 * PI / float(x)),  len / 2.0, -r *sin(i * 2 * PI / float(x))));
	}
	Vertex.push_back(QVector3D(0, -len / 2.0, 0));
	Vertex.push_back(QVector3D(0, len / 2.0, 0));
	
	for (int i = 0; i < 2 * x; i += 2) {
		index.push_back(i);
		index.push_back(i + 2 < 2 * x ? i + 2 : 0);
		index.push_back(i + 1);
		index.push_back(i + 2 < 2 * x ? i + 2 : 0);
		index.push_back(i + 2 < 2 * x ? i + 3 : 1);
		index.push_back(i + 1);
	}

	for (int i = 0; i < 2 * x; i += 2) {
		index.push_back(2 * x);
		index.push_back(i);
		index.push_back(i < 2 * x - 2 ? i + 2 : 0);
		index.push_back(2 * x + 1);
		index.push_back(i + 1);
		index.push_back(i < 2 * x - 2 ? i + 3 : 0);
	}

}

cylinder::~cylinder()
{
}
