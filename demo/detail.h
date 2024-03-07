#pragma once
#include "headers.h"
#include "Mesh.h"

class detail : public QOpenGLWidget, QOpenGLExtraFunctions
{
	Q_OBJECT
public:
	explicit detail(Mesh* m, QWidget* parent = nullptr);		//构造函数
	~detail();

	//鼠标交互事件重写
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	//鼠标变量
	bool changeview_ = false;           //改变视角的标志位
	bool moveModel = false;
	float last_x;						//记录鼠标按下时鼠标的位置用以作视角偏移
	float last_y;
	float yaw = 0.0;					//偏航角
	float pitch = 0.0;					//俯视角
	float fov = 60.0f;					//视野范围
	float sensitivity = 1.5;			//鼠标灵敏度
	float cameraSpeed = 0.1f;
	float r = 10.0f;
	float near_ = 0.01f;
	float far_ = 20.0f;
	QVector3D cameraDirection = QVector3D(0.0f, 0.0f, r);  //摄像机位置
	QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);         //摄像机上向量

	QMatrix4x4 view;
	QMatrix4x4 projection;

	//坐标系绘制参数
	QMatrix4x4 Axis_model;
	const std::vector<float> AxisV = {
		0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		10.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 10.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 10.0f,  0.0f, 0.0f, 1.0f,
	};
	QOpenGLBuffer Axis_vbo;
	QOpenGLShaderProgram* Axis_program;
	void initAxis();
	void drawAxis();

	Mesh* m;
	GLuint vao, vbo, ebo;
	GLuint diffuseTexture;  //漫反射纹理
	QOpenGLShaderProgram* Mesh_program;
	void initmesh();
	void drawmesh();

protected:
	//重载几个opengl的重要函数
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
};
