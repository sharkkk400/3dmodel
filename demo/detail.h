#pragma once
#include "headers.h"
#include "Mesh.h"

class detail : public QOpenGLWidget, QOpenGLExtraFunctions
{
	Q_OBJECT
public:
	explicit detail(Mesh* m, QWidget* parent = nullptr);		//���캯��
	~detail();

	//��꽻���¼���д
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

	//������
	bool changeview_ = false;           //�ı��ӽǵı�־λ
	bool moveModel = false;
	float last_x;						//��¼��갴��ʱ����λ���������ӽ�ƫ��
	float last_y;
	float yaw = 0.0;					//ƫ����
	float pitch = 0.0;					//���ӽ�
	float fov = 60.0f;					//��Ұ��Χ
	float sensitivity = 1.5;			//���������
	float cameraSpeed = 0.1f;
	float r = 10.0f;
	float near_ = 0.01f;
	float far_ = 20.0f;
	QVector3D cameraDirection = QVector3D(0.0f, 0.0f, r);  //�����λ��
	QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);         //�����������

	QMatrix4x4 view;
	QMatrix4x4 projection;

	//����ϵ���Ʋ���
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
	GLuint diffuseTexture;  //����������
	QOpenGLShaderProgram* Mesh_program;
	void initmesh();
	void drawmesh();

protected:
	//���ؼ���opengl����Ҫ����
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
};
