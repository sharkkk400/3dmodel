#include "square.h"

square::square(float x, float z)
{
	Vertex.push_back(QVector3D(x / 2.0, 0, z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, 0, z / 2.0));
	Vertex.push_back(QVector3D(x / 2.0, 0, -z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, 0, -z / 2.0));

	index.push_back(0);
	index.push_back(2);
	index.push_back(3);
	index.push_back(0);
	index.push_back(3);
	index.push_back(1);

}

square::~square()
{
}

