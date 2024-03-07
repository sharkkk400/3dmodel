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
	explicit m_qopenglwidget(QWidget* parent = nullptr);		//构造函数
	~m_qopenglwidget();

	//鼠标变量
	bool changeview_ = false;           //改变视角的标志位
	bool movemodel_ = false;
	bool moveview_ = false;
	bool drawselectframe = false;
	bool selectmode = false;
	meshNode* transnode = nullptr;
	float last_x;						//记录鼠标按下时鼠标的位置用以作视角偏移
	float last_y;
	float yaw = 0.0;					//偏航角
	float pitch = 0.0;					//俯视角
	float h = 0.0;						
	float w = 0.0;						

	//鼠标交互事件重写
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void checkyawpitch();
	QVector4D worldPositionFromMousePosition(const QPoint &pos); //屏幕坐标转世界坐标
	QVector2D viewportXYFromMousePosition(const QPoint &pos);
	QVector3D getTargetPosition(QVector4D worldPosition);
	QPixmap ChangeQImageBackTransparency(QString path);
	void drawStatus();
	QLabel *wps;

	//自定义的meshes核心对象
	std::map<QString, Mesh> meshMap;
	std::map<QString, int> nameNum;
	void LoadMesh(QString filepath, bool adapt = false);
	void selfadapt(Mesh* mesh);
	meshNode* Scene;	//场景根节点
	void unselectallmesh(meshNode* node);
	std::map<QString, meshNode*> nodeMap;
	meshNode* position2node(QVector4D worldPosition);
	std::set<meshNode*> selectedNodes;
	QVector3D selectedCenter;
	
	//光源绘制参数
	sphere light;
	void initLight();
	void drawLight();

	//坐标系绘制参数
	Axis axis;
	Axis smallaxis;
	void initAxis();
	void drawAxis();
	

	//天空盒绘制参数
	const std::vector<float> SkyboxV = {
	-1.0f,-1.0f,-1.0f,   
	  1.0f,-1.0f,-1.0f,   
	  -1.0f,-1.0f, 1.0f,   
	  1.0f,-1.0f,-1.0f,   
	  1.0f,-1.0f, 1.0f,   
	  -1.0f,-1.0f, 1.0f, 
	  // 上
	 -1.0f, 1.0f,-1.0f,   
	 -1.0f, 1.0f, 1.0f,   
	  1.0f, 1.0f,-1.0f,  
	  1.0f, 1.0f,-1.0f,  
	 -1.0f, 1.0f, 1.0f,  
	  1.0f, 1.0f, 1.0f,  
	  // 前
	 -1.0f,-1.0f, 1.0f,   
	  1.0f,-1.0f, 1.0f,   
	 -1.0f, 1.0f, 1.0f,  
	  1.0f,-1.0f, 1.0f,  
	  1.0f, 1.0f, 1.0f,   
	 -1.0f, 1.0f, 1.0f,   
	 // 后
	-1.0f,-1.0f,-1.0f,   
	-1.0f, 1.0f,-1.0f,   
	 1.0f,-1.0f,-1.0f,   
	 1.0f,-1.0f,-1.0f,   
	-1.0f, 1.0f,-1.0f,   
	 1.0f, 1.0f,-1.0f,   
	 // 左
	-1.0f,-1.0f, 1.0f,   
	-1.0f, 1.0f,-1.0f,  
	-1.0f,-1.0f,-1.0f,  
	-1.0f,-1.0f, 1.0f,  
	-1.0f, 1.0f, 1.0f,   
	-1.0f, 1.0f,-1.0f,  
	// 右
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

	//平面地板
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

	//复合模型的外接框
	modelCube* compositeCube = NULL;
	void initcompositeCube(const meshNode* node);
	void drawcompositeCube();

	//几何变换按钮
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
	//重载几个opengl的重要函数
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

};

