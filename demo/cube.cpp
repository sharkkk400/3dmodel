#include "cube.h"

cube::cube(float x, float y, float z)
{
	Vertex.push_back(QVector3D(x / 2.0, y / 2.0, z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, y / 2.0, z / 2.0));
	Vertex.push_back(QVector3D(x / 2.0, y / 2.0, -z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, y / 2.0, -z / 2.0));

	Vertex.push_back(QVector3D(x / 2.0, -y / 2.0, z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, -y / 2.0, z / 2.0));
	Vertex.push_back(QVector3D(x / 2.0, -y / 2.0, -z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, -y / 2.0, -z / 2.0));

	//上
	index.push_back(0);
	index.push_back(2);
	index.push_back(3);
	index.push_back(0);
	index.push_back(3);
	index.push_back(1);

	//下
	index.push_back(4);
	index.push_back(6);
	index.push_back(7);
	index.push_back(4);
	index.push_back(7);
	index.push_back(5);

	//前
	index.push_back(1);
	index.push_back(0);
	index.push_back(4);
	index.push_back(1);
	index.push_back(4);
	index.push_back(5);

	//后
	index.push_back(3);
	index.push_back(2);
	index.push_back(6);
	index.push_back(3);
	index.push_back(6);
	index.push_back(7);

	//左
	index.push_back(3);
	index.push_back(1);
	index.push_back(5);
	index.push_back(3);
	index.push_back(5);
	index.push_back(7);

	//右
	index.push_back(0);
	index.push_back(2);
	index.push_back(6);
	index.push_back(0);
	index.push_back(6);
	index.push_back(4);
}

cube::~cube()
{
}
