#pragma once
#include "headers.h"

class meshComponent : public QOpenGLExtraFunctions
{
public:
	meshComponent();
	~meshComponent();

	std::vector<QVector3D> Vertex;
	std::vector<QVector2D> ScreenVertex;
	std::vector<int> index;
	std::vector<float> boxCoord;

	QMatrix4x4 meshModel;						//局部坐标系变换矩阵
	QMatrix4x4 baseModel;						//基态坐标系变换矩阵
	QMatrix4x4 model;							//世界坐标系变换矩阵

	void getwpsboxcoord();
	bool isSelect = false;
	bool checkSelect(QVector4D worldPosition);
	bool checkSelect(QVector2D viewportXY);

	GLuint vao, vbo, ebo;
	QOpenGLShaderProgram* program;
	QVector3D color;
	double linewidth;
	bool Topping;
	float alpha;
	float viewportXmax;
	float viewportXmin;
	float viewportYmax;
	float viewportYmin;
	void updatevieportXY();

	void bind();
	void draw(QMatrix4x4 base = QMatrix4x4());
	int drawitem;

	inline float crossProduct(QVector2D v1, QVector2D v2) {
		return v1.x() * v2.y() - v2.x() * v1.y();
	}
};

