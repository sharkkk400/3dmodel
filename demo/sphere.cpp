#include"sphere.h"

sphere::sphere(float r, int x)
{
	Vertex.clear();
	for (int i = 0; i <= x; i++) {
		float _x = r * cos(i / float(x) * PI);
		for (int j = 0; j < 2 * x; j++) {
			float _r = r * sin(i / float(x) * PI);
			float _y = _r * cos(j / float(x) * PI);
			float _z = _r * sin(j / float(x) * PI);
			Vertex.push_back(QVector3D(_x, _y, _z));
		}
	}

	index.clear();
	for (int i = 0; i < x; i++) {
		int begin = i * 2 * x;
		int end = (i + 1) * 2 * x;
		for (int j = 0; j < 2 * x; j++) {
			index.push_back(begin + j);
			index.push_back(begin + j + 2 * x);
			index.push_back((begin + j + 1) == end ? (end) : (begin + j + 2 * x + 1));
			index.push_back(begin + j);
			index.push_back((begin + j + 1) == end ? (end) : (begin + j + 2 * x + 1));
			index.push_back((begin + j + 1) == end ? begin : (begin + j + 1));
		}
	}
}

sphere::~sphere()
{
}
