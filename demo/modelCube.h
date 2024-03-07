#pragma once
#include "headers.h"
#include "meshNode.h"
class modelCube : public QOpenGLExtraFunctions
{
public:
	modelCube();
	~modelCube();

	std::vector<float> coords;			//x1,y1,z1,x2,y2,z2
	std::vector<QVector3D> edges;		//��
	QVector3D center;

	GLuint cube_vao, cube_vbo;
	QOpenGLShaderProgram* cube_program;


	//Ϊ�ڵ㴴����ӿ�
	void initdata(const meshNode* node);
	void loadcoords(const meshNode* node);

	//Ϊmesh������ӿ�
	void initdata(std::vector<float> &coordinates);
	void coords2edges(std::vector<float> &coordinates);

	void binddata();

	void initdata(const QList<Mesh*> &meshes);
	void loadcoords(const QList<Mesh*> &meshes);
	void coords2edges();



	void draw(QMatrix4x4 model = QMatrix4x4(),QVector3D color = QVector3D(1,1,1));


};

