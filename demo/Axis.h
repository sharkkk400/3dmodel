#pragma once
#include "headers.h"
#include "parameter.h"
class Axis : public QOpenGLExtraFunctions
{
public:
	Axis(float len = 5 ,float r = 0.1 ,float h = 0.2, int x = 15);
	~Axis();

	float len;
	float r;
	float h;
	int x;
	std::vector<QVector3D> Vertex;
	std::vector<int> index;
	QMatrix4x4 m;
	void init();
	void bind();
	void draw(QMatrix4x4 base, QMatrix4x4 proj = parameter::projection, QMatrix4x4 view = parameter::view);

	GLuint cone_vao, cone_vbo, cone_ebo;
	QOpenGLShaderProgram* cone_program;
};

