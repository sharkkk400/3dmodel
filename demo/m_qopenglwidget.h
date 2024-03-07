#pragma once
#include "headers.h"
#include "Model.h"
#include "modelCube.h"
#include "meshNode.h"
#include "Axis.h"
#include "mButton.h"
#include "moveAxis.h"
#include "rotateAxis.h"
#include "scaleAxis.h"
#include "transAxis.h"
#include "square.h"
#include "square_tex.h"
#include "SelectFrame.h"
#include "dashline.h"
#include "line.h"

class m_qopenglwidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit m_qopenglwidget(QWidget* parent = nullptr);		//���캯��
	~m_qopenglwidget();

	//������
	bool changeview_ = false;           //�ı��ӽǵı�־λ
	bool movemodel_ = false;
	bool moveview_ = false;
	bool drawselectframe = false;
	bool selectmode = false;
	meshNode* transnode = nullptr;
	float last_x;						//��¼��갴��ʱ����λ���������ӽ�ƫ��
	float last_y;
	float yaw = 0.0;					//ƫ����
	float pitch = 0.0;					//���ӽ�
	float h = 0.0;						
	float w = 0.0;						

	//��꽻���¼���д
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void checkyawpitch();
	QVector4D worldPositionFromMousePosition(const QPoint &pos); //��Ļ����ת��������
	QVector2D viewportXYFromMousePosition(const QPoint &pos);
	QVector3D getTargetPosition(QVector4D worldPosition);
	QPixmap ChangeQImageBackTransparency(QString path);
	void drawStatus();
	QLabel *wps;

	//�Զ����meshes���Ķ���
	std::map<QString, Mesh> meshMap;
	std::map<QString, int> nameNum;
	void LoadMesh(QString filepath, bool adapt = false);
	void selfadapt(Mesh* mesh);
	meshNode* Scene;	//�������ڵ�
	void unselectallmesh(meshNode* node);
	std::map<QString, meshNode*> nodeMap;
	meshNode* position2node(QVector4D worldPosition);
	std::set<meshNode*> selectedNodes;
	QVector3D selectedCenter;
	
	//��Դ���Ʋ���
	sphere light;
	void initLight();
	void drawLight();

	//����ϵ���Ʋ���
	Axis axis;
	Axis smallaxis;
	void initAxis();
	void drawAxis();
	

	//��պл��Ʋ���
	const std::vector<float> SkyboxV = {
	-1.0f,-1.0f,-1.0f,   
	  1.0f,-1.0f,-1.0f,   
	  -1.0f,-1.0f, 1.0f,   
	  1.0f,-1.0f,-1.0f,   
	  1.0f,-1.0f, 1.0f,   
	  -1.0f,-1.0f, 1.0f, 
	  // ��
	 -1.0f, 1.0f,-1.0f,   
	 -1.0f, 1.0f, 1.0f,   
	  1.0f, 1.0f,-1.0f,  
	  1.0f, 1.0f,-1.0f,  
	 -1.0f, 1.0f, 1.0f,  
	  1.0f, 1.0f, 1.0f,  
	  // ǰ
	 -1.0f,-1.0f, 1.0f,   
	  1.0f,-1.0f, 1.0f,   
	 -1.0f, 1.0f, 1.0f,  
	  1.0f,-1.0f, 1.0f,  
	  1.0f, 1.0f, 1.0f,   
	 -1.0f, 1.0f, 1.0f,   
	 // ��
	-1.0f,-1.0f,-1.0f,   
	-1.0f, 1.0f,-1.0f,   
	 1.0f,-1.0f,-1.0f,   
	 1.0f,-1.0f,-1.0f,   
	-1.0f, 1.0f,-1.0f,   
	 1.0f, 1.0f,-1.0f,   
	 // ��
	-1.0f,-1.0f, 1.0f,   
	-1.0f, 1.0f,-1.0f,  
	-1.0f,-1.0f,-1.0f,  
	-1.0f,-1.0f, 1.0f,  
	-1.0f, 1.0f, 1.0f,   
	-1.0f, 1.0f,-1.0f,  
	// ��
	1.0f,-1.0f, 1.0f,  
	1.0f,-1.0f,-1.0f, 
	1.0f, 1.0f,-1.0f,  
	1.0f,-1.0f, 1.0f, 
	1.0f, 1.0f,-1.0f,  
	1.0f, 1.0f, 1.0f,  
	};
	void initSkybox();
	void drawSkybox();
	QOpenGLVertexArrayObject Skybox_vao;
	QOpenGLBuffer Skybox_vbo;
	QOpenGLBuffer Skybox_ebo;
	QOpenGLShaderProgram* Skybox_program;
	QOpenGLTexture *m_cubeMap;
	QMatrix4x4 Skybox_model;

	//ƽ��ذ�
	QMatrix4x4 Ground_model;
	const std::vector<float> GroundV = {
		-50.0f, 0.0f, -50.0f, 0.0f, 0.0f,
		-49.0f, 0.0f, -50.0f, 1.0f, 0.0f,
		-50.0f, 0.0f, -49.0f, 0.0f, 1.0f,
		-49.0f, 0.0f, -50.0f, 1.0f, 0.0f,
		-49.0f, 0.0f, -49.0f, 1.0f, 1.0f,
		-50.0f, 0.0f, -49.0f, 0.0f, 1.0f,
	};
	void initGround();
	void drawGround();
	QOpenGLVertexArrayObject Ground_vao;
	QOpenGLBuffer Ground_vbo;
	QOpenGLShaderProgram* Ground_program;
	QOpenGLTexture *Ground_Texture;

	QLabel* imageLabel;
	square_tex statusPng;
	void initPng();
	void drawPng();

	//����ģ�͵���ӿ�
	modelCube* compositeCube = NULL;
	void initcompositeCube(const meshNode* node);
	void drawcompositeCube();

	//���α任��ť
	int transformItem;
	std::vector<mButton*>  transformbtns;
	mButton* selectbtn;
	mButton* scalebtn;
	mButton* rotatebtn;
	mButton* movebtn;
	void inittransformButtons();
	moveAxis mAxis;
	rotateAxis rAxis;
	scaleAxis sAxis;
	std::map<int, transAxis*> transAxisMap;
	void initTransAxis();
	void drawTransAxis();
	void undrawallTransAxis();

	

	SelectFrame* selectframe = NULL;
	QPoint v1;
	QPoint v2;
	QPoint v3;
	QPoint v4;
	void initsf();
	void drawsf();


signals:
	void updateTree(QString name);
	void updateViewpoint(QVector3D viewpositon);

public slots:
	void handleMeshes(std::vector<Mesh>);
	void handlebtn(int index);
protected:
	//���ؼ���opengl����Ҫ����
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

};

