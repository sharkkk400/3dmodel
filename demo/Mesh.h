#pragma once
#include "headers.h"
#include "Axis.h"


class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh();
	~Mesh();

	//数据绑定，给gpu传入vao、vbo、ebo
	bool isbind = false;
	void bindData();

	// 初始化着色器
	void initShaderProgram();

	//绘制mesh，参数为基态变换矩阵
	void draw();

	//初始化变换矩阵
	void initModel(QMatrix4x4 base= QMatrix4x4(), QVector3D parentBox = QVector3D(0, 0, 0));

	//父子矩阵矫正
	void correctModel(QMatrix4x4 base = QMatrix4x4());

	GLuint vao, vbo, ebo;
	GLuint diffuseTexture;  //漫反射纹理
	QOpenGLShaderProgram* Mesh_program;
	QOpenGLShaderProgram* MeshBox_program;

	std::vector<QVector3D> vertexPosition;		// 顶点位置坐标
	std::vector<QVector2D> vertexTexcoord;		// 顶点纹理坐标
	std::vector<QVector3D> vertexNormal;		// 顶点法线坐标
	std::vector<int> index;						// glDrawElements 函数的绘制索引

	QMatrix4x4 meshModel;						//局部坐标系变换矩阵
	QMatrix4x4 baseModel;						//基态坐标系变换矩阵
	QMatrix4x4 model;							//世界坐标系变换矩阵

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
	bool isSelect = false;						//是否选中，若选中则显示外接立方体
	bool checkSelect(QVector4D worldPosition);
	bool checkSelect(QVector3D linedir);
	void moveModel(QVector4D newwps);

	//局部坐标轴
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

