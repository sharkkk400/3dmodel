#pragma once
#include "headers.h"
#include "Axis.h"


class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh();
	~Mesh();

	//���ݰ󶨣���gpu����vao��vbo��ebo
	bool isbind = false;
	void bindData();

	// ��ʼ����ɫ��
	void initShaderProgram();

	//����mesh������Ϊ��̬�任����
	void draw();

	//��ʼ���任����
	void initModel(QMatrix4x4 base= QMatrix4x4(), QVector3D parentBox = QVector3D(0, 0, 0));

	//���Ӿ������
	void correctModel(QMatrix4x4 base = QMatrix4x4());

	GLuint vao, vbo, ebo;
	GLuint diffuseTexture;  //����������
	QOpenGLShaderProgram* Mesh_program;
	QOpenGLShaderProgram* MeshBox_program;

	std::vector<QVector3D> vertexPosition;		// ����λ������
	std::vector<QVector2D> vertexTexcoord;		// ������������
	std::vector<QVector3D> vertexNormal;		// ���㷨������
	std::vector<int> index;						// glDrawElements �����Ļ�������

	QMatrix4x4 meshModel;						//�ֲ�����ϵ�任����
	QMatrix4x4 baseModel;						//��̬����ϵ�任����
	QMatrix4x4 model;							//��������ϵ�任����

	QVector3D target;
	QVector3D target_wps;
	QVector3D moveVctor;
	float target_wpsX;
	float target_wpsY;
	float target_wpsZ;
	double scaleX = 1;
	double scaleY = 1;
	double scaleZ = 1;
	double translateX = 0;
	double translateY = 0;
	double translateZ = 0;
	QVector3D translate = QVector3D(0.0f, 0.0f, 0.0f);
	float rotateX = 0.0;
	float rotateY = 0.0;
	float rotateZ = 0.0;


	GLuint box_vao, box_vbo;
	std::vector<float> boxCoord;				//X1,Y1,Z1,X2,Y2,Z2
	void getwpsboxcoord();
	std::vector<QVector3D> boxVertex;
	QVector3D boxCenter;
	QVector3D boxCenter_wps;
	bool isSelect = false;						//�Ƿ�ѡ�У���ѡ������ʾ���������
	bool checkSelect(QVector4D worldPosition);
	bool checkSelect(QVector3D linedir);
	void moveModel(QVector4D newwps);

	//�ֲ�������
	Axis Axis_mesh;
	float lenth;

	bool flag = false;
	QString meshName;
	QString rootpath;
	QVector3D objectColor = QVector3D(0.3451, 0.5647, 0.8824);
	QVector3D Ka;
	QVector3D Kd;
	QVector3D Ks;
	float Ns = 32;
	void updateModel();
};

