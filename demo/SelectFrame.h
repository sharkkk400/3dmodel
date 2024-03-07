#pragma once
#include "headers.h"
#include "meshNode.h"

class SelectFrame : public QOpenGLExtraFunctions
{
public:
	SelectFrame(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4);
	~SelectFrame();

	GLuint polygon_vao, polygon_vbo, frame_vao, frame_vbo;
	QOpenGLShaderProgram* polygon_program;
	QOpenGLShaderProgram* frame_program;

	std::vector<QVector3D> polygonV;
	std::vector<QVector3D> frameV;
	std::vector<QVector3D> rayV;
	void init();
	void draw();

	void checkin(std::map<QString, meshNode*> &nodeMap, std::set<meshNode*> &selectedNodes);
};

