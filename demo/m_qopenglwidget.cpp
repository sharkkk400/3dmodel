#include "m_qopenglwidget.h"

m_qopenglwidget::m_qopenglwidget(QWidget* parent)
{
	this->setWindowFlag(Qt::FramelessWindowHint);

	inittransformButtons();

	wps = new QLabel(this);
	wps->setText("X:,Y:,Z:");

	Scene = new meshNode();
	Scene->isDraw = false;

	setMouseTracking(true);
}

m_qopenglwidget::~m_qopenglwidget()
{
}

void m_qopenglwidget::mouseMoveEvent(QMouseEvent * event)
{
	makeCurrent();
	
	QVector4D worldPosition = worldPositionFromMousePosition(event->pos());

	wps->setText("viewX:" + QString::number(viewportXYFromMousePosition(event->pos()).x()) + "viewY:" + QString::number(viewportXYFromMousePosition(event->pos()).y()) + "||X:" + QString::number(worldPosition.x()) + ",Y:" + QString::number(worldPosition.y()) + ",Z:" + QString::number(worldPosition.z()));
	
	float offset_x = event->x() - last_x;
	float offset_y = event->y() - last_y;

		//移动视角
	if (changeview_) {
		if (offset_x < 0) {
			yaw += parameter::sensitivity * 1 * PI / 180;
		}
		else if (offset_x > 0) {
			yaw -= parameter::sensitivity * 1 * PI / 180;
		}
		if (offset_y > 0) {
			pitch += parameter::sensitivity * 1 * PI / 180;
		}
		else if (offset_y < 0) {
			pitch -= parameter::sensitivity * 1 * PI / 180;
		}
		checkyawpitch();
		parameter::cameraDirection.setX(parameter::r * sin(yaw)*cos(pitch));
		parameter::cameraDirection.setY(parameter::r * sin(pitch));
		parameter::cameraDirection.setZ(parameter::r * cos(yaw)*cos(pitch));

	}
	if (moveview_) {
		w = 0;
		h = 0;
		if (offset_y > 0) {
			h += parameter::cameraSpeed;
		}
		else if (offset_y < 0) {
			h -= parameter::cameraSpeed;
		}
		if (offset_x < 0) {
			w += parameter::cameraSpeed;
		}
		else if (offset_x > 0) {
			w -= parameter::cameraSpeed;
		}
		QVector3D p(0, 0, 0);
		QMatrix4x4 m;
		m.rotate(yaw * 180 / PI, 0, 1, 0);
		m.rotate(-pitch * 180 / PI, 1, 0, 0);
		m.translate(w, h, 0);
		p = m * p;
		parameter::viewDirection += p;
		emit updateViewpoint(parameter::viewDirection);
	}
	
	if (selectmode) {
		v3 = event->pos();
		initsf();
	}

	if (!movemodel_ && transformItem != 0&& transAxisMap[transformItem]->isDraw) {
		transAxisMap[transformItem]->checkinitem(viewportXYFromMousePosition(event->pos()));
	}
	if (movemodel_ && mAxis.item == 0) {
		QVector3D tagetPosition = getTargetPosition(worldPosition);
		transnode->mesh->moveModel(transnode->mesh->baseModel.inverted()*tagetPosition);
		transnode->updateModel();
		initcompositeCube(transnode);
		mAxis.base.setToIdentity();
		mAxis.base.translate(transnode->mesh->model * transnode->mesh->boxCenter);
	}
	update();
	last_x = event->x();
	last_y = event->y();
}

void m_qopenglwidget::mousePressEvent(QMouseEvent * event)
{
	//鼠标右键事件：旋转视角、平移视角
	if (event->button() == Qt::RightButton) {
		//鼠标右键变换视线方向
		if (event->modifiers() != Qt::ShiftModifier) {
			changeview_ = true;
			moveview_ = false;
		}
		//鼠标右键+Shift移动视点位置
		else {
			changeview_ = false;
			moveview_ = true;
		}
	}

	//鼠标左键事件：根据变换按钮来进行操作
	if (event->button() == Qt::LeftButton) {
		makeCurrent();
		QVector4D worldPosition;
		worldPosition = worldPositionFromMousePosition(event->pos());
		wps->setText("viewX:"+ QString::number(viewportXYFromMousePosition(event->pos()).x())+ "viewY:" + QString::number(viewportXYFromMousePosition(event->pos()).y()) + "||X:" + QString::number(worldPosition.x()) + ",Y:" + QString::number(worldPosition.y()) + ",Z:" + QString::number(worldPosition.z()));
		
		/*
		//选择模型
		meshNode* node = position2node(worldPosition);

		//已选定变换节点
		if (transnode) {
			if (transformItem == 3 &&
				transAxisMap[transformItem]->isDraw &&
				transAxisMap[transformItem]->checkinitem(viewportXYFromMousePosition(event->pos()))) {
				movemodel_ = true;
			}
			else if (node != transnode && node->mesh) {
				unselectallmesh(Scene);
				transnode = node;
				node->mesh->isSelect = true;
				if (transformItem != 0) {
					transAxisMap[transformItem]->base.setToIdentity();
					transAxisMap[transformItem]->base.translate(node->mesh->model * node->mesh->boxCenter);
					transAxisMap[transformItem]->isDraw = true;
				}
			}
			else if (node != transnode && !node->mesh) {
				transnode = nullptr;
				unselectallmesh(Scene);
				undrawallTransAxis();
			}
		}
		//未选定变换节点
		else {
			if (node->mesh) {
				transnode = node;
				node->mesh->isSelect = true;
				if (transformItem != 0) {
					transAxisMap[transformItem]->base.setToIdentity();
					transAxisMap[transformItem]->base.translate(node->mesh->model * node->mesh->boxCenter);
					transAxisMap[transformItem]->isDraw = true;
				}
				
			}
			else {
				unselectallmesh(Scene);
				undrawallTransAxis();
			}
		}
		initcompositeCube(node);*/
		
		//选择模型
		if (!movemodel_) {
			selectedNodes.clear();
			v1 = event->pos();
			selectmode = true;
		}
		
	}
	
	//更新视图
	update();
}

void m_qopenglwidget::mouseReleaseEvent(QMouseEvent * event)
{
	makeCurrent();
	if (selectmode) {
		//读取选取框内容
		if (selectframe) {
			selectframe->checkin(nodeMap, selectedNodes);
			delete selectframe;
			selectframe = NULL;
		}
		else {
			meshNode* node = position2node(worldPositionFromMousePosition(event->pos()));
			selectedNodes.insert(node);
		}
		//更新视图
		unselectallmesh(Scene);
		undrawallTransAxis();
		for (auto node : selectedNodes) {
			if (node->mesh) {
				node->mesh->isSelect = true;
			}
		}
	}

	//恢复默认状态
	changeview_ = false;
	moveview_ = false;
	movemodel_ = false;
	drawselectframe = false;
	selectmode = false;
	if (transformItem!=0) {
		transAxisMap[transformItem]->item = 0;
	}
	update();
}

void m_qopenglwidget::wheelEvent(QWheelEvent * event)
{
	if (event->delta() > 0) {
		parameter::r = parameter::r - parameter::cameraSpeed;
	}
	else {
		parameter::r = parameter::r + parameter::cameraSpeed;
	}
	parameter::cameraDirection.setX(parameter::r * sin(yaw)*cos(pitch));
	parameter::cameraDirection.setY(parameter::r * sin(pitch));
	parameter::cameraDirection.setZ(parameter::r * cos(yaw)*cos(pitch));
	this->repaint();
}

void m_qopenglwidget::keyReleaseEvent(QKeyEvent * event)
{
	makeCurrent();
	float exyaw = 0.0;
	float expitch = 0.0;


	if (event->key() == Qt::Key_4) {
		exyaw = -PI/6.0;
	}
	if (event->key() == Qt::Key_6) {
		exyaw = PI /6.0;
	}
	if (event->key() == Qt::Key_8) {
		expitch = PI / 6.0;
	}
	if (event->key() == Qt::Key_5) {
		expitch = -PI / 6.0;
	}
	if (event->key() == Qt::Key_1) {
		exyaw = PI / 2.0 - yaw;
		expitch = 0.0 - pitch;
		parameter::viewDirection = QVector3D(0, 0, 0);
		emit updateViewpoint(QVector3D(0, 0, 0));
	}
	if (event->key() == Qt::Key_2) {
		exyaw = 0.0 - yaw;
		expitch = 0.0 - pitch;
		parameter::viewDirection = QVector3D(0, 0, 0);
		emit updateViewpoint(QVector3D(0, 0, 0));
	}
	if (event->key() == Qt::Key_3) {
		exyaw = 0 - yaw;
		expitch = PI / 2.0 - pitch; 
		parameter::viewDirection = QVector3D(0, 0, 0);
		emit updateViewpoint(QVector3D(0, 0, 0));
	}
	for (int i = 0; i < 20; i++) {
		QTimer::singleShot(5*i, this, [=]() {
			yaw += (exyaw / 20.0);
			pitch += (expitch / 20.0);

			checkyawpitch();
			parameter::cameraDirection.setX(parameter::r * sin(yaw)*cos(pitch));
			parameter::cameraDirection.setY(parameter::r * sin(pitch));
			parameter::cameraDirection.setZ(parameter::r * cos(yaw)*cos(pitch));
			
			update();
		});
	}
	
}

void m_qopenglwidget::checkyawpitch()
{
	if (yaw > 2 * PI) {
		yaw = yaw - 2 * PI;
	}
	if (yaw < -2 * PI) {
		yaw = yaw + 2 * PI;
	}

	if (pitch > PI / 2) {
		pitch = PI / 2 - 0.000001;
	}
	if (pitch < -PI / 2) {
		pitch = -PI / 2 + 0.000001;
	}
}

QPixmap m_qopenglwidget::ChangeQImageBackTransparency(QString path)
{
	QImage *img = new QImage;
	img->load(path);
	QPixmap pix = QPixmap::fromImage(*img);
	QPixmap tmpPix(pix.size());
	tmpPix.fill(Qt::transparent);
	QPainter p1(&tmpPix);
	p1.setCompositionMode(QPainter::CompositionMode_Source);
	p1.drawPixmap(0, 0, pix);
	p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	//200表示透明度，数值0表示完全透明，数值255表示不透明
	p1.fillRect(tmpPix.rect(), QColor(0, 0, 0, 200));
	p1.end();
	delete img;
	return tmpPix;
}

void m_qopenglwidget::drawStatus()
{
	// 创建图片标签
	imageLabel = new QLabel(this);
	// 加载图片
	QPixmap pixmap = ChangeQImageBackTransparency(QString::fromStdString(parameter::path) + "data/status.png");

	// 设置图片大小（根据需要调整）
	//pixmap = pixmap.scaled(150, 150, Qt::KeepAspectRatio);

	// 显示图片
	imageLabel->setPixmap(pixmap);

	// 创建布局并放置图片标签
	QHBoxLayout* layout = new QHBoxLayout();
	layout->addWidget(imageLabel);
	layout->setAlignment(Qt::AlignRight | Qt::AlignTop);
	setLayout(layout);
}

QVector4D m_qopenglwidget::worldPositionFromMousePosition(const QPoint & pos)
{
	float winZ;
	glReadPixels((int)pos.x(), this->height() - (int)pos.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	float x = (2.0 * pos.x()) / this->width() - 1.0f;
	float y = 1.0f - (2.0f * pos.y()) / this->height();
	float z = winZ * 2.0 - 1.0f;

	float w = (2.0 * parameter::near_ * parameter::far_) / (parameter::far_ + parameter::near_ - z * (parameter::far_ - parameter::near_));
	QVector4D worldPosition(x, y, z, 1);
	worldPosition *= w;
	worldPosition = parameter::view.inverted() * parameter::projection.inverted() * worldPosition;
	return worldPosition;
}

QVector2D m_qopenglwidget::viewportXYFromMousePosition(const QPoint & pos)
{
	float x = (2.0 * pos.x()) / this->width() - 1.0f;
	float y = 1.0f - (2.0f * pos.y()) / this->height();
	return QVector2D(x,y);
}

QVector3D m_qopenglwidget::getTargetPosition(QVector4D worldPosition)
{
	//计算视线的直线方向向量
	QVector3D directionVector = parameter::viewDirection + parameter::cameraDirection - QVector3D(worldPosition);

	//观察平面法向量
	QVector3D planeNormal = QVector3D(0, 0, 1);
	QMatrix4x4 m;
	m.rotate(yaw * 180 / PI, 0, 1, 0);
	m.rotate(-pitch * 180 / PI, 1, 0, 0);
	planeNormal = m * planeNormal;
	float planeConstant = -QVector3D::dotProduct(planeNormal,parameter::viewDirection);

	//求解参数方程的参数t
	float t = (-planeConstant - QVector3D::dotProduct(QVector3D(worldPosition), planeNormal)) / QVector3D::dotProduct(directionVector, planeNormal);
	
	//得到交点坐标
	return QVector3D(QVector3D(worldPosition)+ t* directionVector);
}

void m_qopenglwidget::handleMeshes(std::vector<Mesh> meshes) {
	for (auto& mesh : meshes) {
		mesh.bindData();
		QString name = mesh.rootpath.section('/', -1);
		if (nameNum.find(name) != nameNum.end()) {
			nameNum[name]++;
			name = name + "(" + QString::number(nameNum[name]) + ")";
		}
		else {
			nameNum.insert(std::pair<QString, int>(name, 1));
		}
		mesh.meshName = name;
		meshMap.insert(std::pair<QString, Mesh>(name, mesh));
		meshNode* node = new meshNode(&meshMap[name], Scene);
		node->name = node->mesh->meshName;
		nodeMap.insert(std::pair<QString, meshNode*>(name, node));
		emit updateTree(name);
		update();
	}
}

void m_qopenglwidget::LoadMesh(QString filepath, bool adapt)
{
	makeCurrent();
	Model m;
	m.Load(std::string(filepath.toLocal8Bit()));

	for (auto& mesh : m.meshes) {
		QString name = filepath.section('/',-1);
		if (nameNum.find(name) != nameNum.end()) {
			nameNum[name]++;
			name = name + "(" + QString::number(nameNum[name]) + ")";
		}
		else {
			nameNum.insert(std::pair<QString, int>(name, 1));
		}
		mesh.meshName = name;
		mesh.rootpath = filepath;
		meshMap.insert(std::pair<QString,Mesh>(name, mesh));
		
		meshNode* node = new meshNode(&meshMap[name], Scene);
		node->name = node->mesh->meshName;
		nodeMap.insert(std::pair<QString, meshNode*>(name, node));
		emit updateTree(name);
		Scene->binddata();
		if (adapt) {
			selfadapt(&meshMap[name]);
		}
		update();
	}
}

void m_qopenglwidget::selfadapt(Mesh * mesh)
{
	mesh->target_wpsX = 0;
	mesh->target_wpsY = 0;
	mesh->target_wpsZ = 0;

	mesh->scaleX = 4.0 / mesh->lenth * 0.6;
	mesh->scaleY = 4.0 / mesh->lenth * 0.6;
	mesh->scaleZ = 4.0 / mesh->lenth * 0.6;

	mesh->updateModel();
}

void m_qopenglwidget::unselectallmesh(meshNode* node)
{
	transnode = nullptr;
	if (node->mesh) {
		node->mesh->isSelect = false;
	}
	if (!node->children.empty()) {
		for (auto child : node->children) {
			unselectallmesh(child);
		}
	}
}

meshNode * m_qopenglwidget::position2node(QVector4D worldPosition)
{
	makeCurrent();
	for (auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
		if (it->second->mesh->checkSelect(worldPosition)) {
			return it->second;
		}
	}
	return Scene;
}

void m_qopenglwidget::initLight()
{
	light.bind();
}

void m_qopenglwidget::drawLight()
{
	light.meshModel.setToIdentity();
	light.meshModel.translate(parameter::lightPosition);
	light.draw();
}

void m_qopenglwidget::initAxis()
{
	axis.init();

	smallaxis.len = 0.8;
	smallaxis.r = 0.1;
	smallaxis.h = 0.2;
	smallaxis.init();
}

void m_qopenglwidget::drawAxis()
{
	if (parameter::isdrawAxis) {
		axis.draw(QMatrix4x4());
	}
	else {
		QVector3D op(-5.5, 4.5, 0);
		QMatrix4x4 m;
		m.setToIdentity();
		m.translate(parameter::viewDirection);
		m.rotate(yaw * 180 / PI, 0, 1, 0);
		m.rotate(-pitch * 180 / PI, 1, 0, 0);
		m.translate(0, 0, parameter::r - 10.0);
		op = m * op;
		QMatrix4x4 base;
		base.translate(op);
		QMatrix4x4 ov;
		smallaxis.draw(base);
	}
	

	
}

void m_qopenglwidget::initSkybox()
{
	Skybox_vao.create();
	Skybox_vbo.create();

	Skybox_vao.bind();
	Skybox_vbo.bind();
	Skybox_vbo.allocate(SkyboxV.data(), SkyboxV.size() * sizeof(SkyboxV));

	Skybox_program = new QOpenGLShaderProgram();
	Skybox_program->bind();

	//创建着色器
	{
		Skybox_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
		#version 330 core
		layout (location = 0) in vec3 vPosition;
		out vec3 texcoord;
		uniform mat4 view;
		uniform mat4 projection;
		void main(){
			vec4 pos = projection * view * vec4(vPosition, 1.0);  
			gl_Position = pos.xyww;
			texcoord = vPosition;
		})");
		Skybox_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
		#version 330 core
		in vec3 texcoord;
		out vec4 fColor;
		uniform samplerCube skybox;
		void main(){
			fColor = textureCube(skybox, texcoord);
		})");
	}
	Skybox_program->link();
	Skybox_program->setAttributeBuffer(Skybox_program->attributeLocation("vPosition"), GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
	Skybox_program->enableAttributeArray(Skybox_program->attributeLocation("vPosition"));

	m_cubeMap = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
	QImage _right = QImage(QString::fromStdString(parameter::path) + "data/skybox/right.jpg").convertToFormat(QImage::Format_RGB888);
	QImage _left = QImage(QString::fromStdString(parameter::path) + "data/skybox/left.jpg").convertToFormat(QImage::Format_RGB888);
	QImage _top = QImage(QString::fromStdString(parameter::path) + "data/skybox/top.jpg").convertToFormat(QImage::Format_RGB888);
	QImage _bottom = QImage(QString::fromStdString(parameter::path) + "data/skybox/bottom.jpg").convertToFormat(QImage::Format_RGB888);
	QImage _front = QImage(QString::fromStdString(parameter::path) + "data/skybox/front.jpg").convertToFormat(QImage::Format_RGB888);
	QImage _back = QImage(QString::fromStdString(parameter::path) + "data/skybox/back.jpg").convertToFormat(QImage::Format_RGB888);

	// 设置宽高
	m_cubeMap->setSize(_right.width(), _right.height());
	// 设置颜色模式
	m_cubeMap->setFormat(QOpenGLTexture::RGBFormat);
	// 分配存储区域
	m_cubeMap->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);
	// 分别设置六个面的纹理数据
	m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_right.bits());
	m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_left.bits());
	m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_top.bits());
	m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_bottom.bits());
	m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_front.bits());
	m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_back.bits());

	//纹理放大或缩小时，像素的取值方法 ，线性或就近抉择
	m_cubeMap->setMinificationFilter(QOpenGLTexture::Linear);
	m_cubeMap->setMagnificationFilter(QOpenGLTexture::Linear);
	//设置纹理边缘的扩展方法
	m_cubeMap->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
	m_cubeMap->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

	Skybox_ebo.release();
	Skybox_vbo.release();
	Skybox_vao.release();

	Skybox_program->release();

}

void m_qopenglwidget::drawSkybox()
{
	glDepthFunc(GL_LEQUAL);
	Skybox_program->bind();
	QMatrix4x4 skyboxView = parameter::view;
	// 移除skyboxView的位移属性
	skyboxView.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
	Skybox_program->setUniformValue("projection", parameter::projection);
	Skybox_program->setUniformValue("view", skyboxView);

	Skybox_program->setUniformValue("skybox", 0);
	m_cubeMap->bind();

	Skybox_vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	Skybox_vao.release();

	// 深度测试函数设置为默认行为
	glDepthFunc(GL_LESS);
	Skybox_program->release();

}

void m_qopenglwidget::initGround()
{
	Ground_vao.create();
	Ground_vbo.create();

	Ground_vao.bind();
	Ground_vbo.bind();

	Ground_vbo.allocate(GroundV.data(), GroundV.size() * sizeof(GroundV));

	Ground_program = new QOpenGLShaderProgram();
	Ground_program->bind();
	//添加顶点着色器
	Ground_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTextureCoord;
	out vec3 outColor;
	out vec2 textureCoord;
	uniform mat4 model = mat4(1.0);
	uniform mat4 view = mat4(1.0);
	uniform mat4 projection = mat4(1.0);
	void main(){
		gl_Position = projection * view * model * vec4(aPos, 1.0);
		textureCoord = aTextureCoord;
	})");
	//添加片段着色器
	Ground_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	out vec4 fragColor;
	in vec3 outColor;//从顶点着色器中传过来的颜色
	in vec2 textureCoord;
	uniform sampler2D textureImg;
	void main(){
		vec4 texColor = texture(textureImg, textureCoord);
		if(texColor.a < 0.1)
			discard;
		fragColor = texColor;
	})");
	Ground_program->link();

	Ground_program->setAttributeBuffer(Ground_program->attributeLocation("aPos"), GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
	Ground_program->enableAttributeArray(Ground_program->attributeLocation("aPos"));
	Ground_program->setAttributeBuffer(Ground_program->attributeLocation("aTextureCoord"), GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
	Ground_program->enableAttributeArray(Ground_program->attributeLocation("aTextureCoord"));

	//垂直镜像mirrored
	Ground_Texture = new QOpenGLTexture(QImage(QString::fromStdString(parameter::path) + "data/texture/frame.png").mirrored());
	Ground_Texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
	Ground_Texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
	Ground_Texture->setMinificationFilter(QOpenGLTexture::Linear);
	Ground_Texture->setMagnificationFilter(QOpenGLTexture::Linear);

	Ground_program->setUniformValue("textureImg", 0);

	Ground_program->release();//解绑程序
	Ground_vbo.release();
	Ground_vao.release();
}

void m_qopenglwidget::drawGround()
{
	Ground_vao.bind();
	Ground_vbo.bind();
	glActiveTexture(GL_TEXTURE0);
	Ground_Texture->bind();

	Ground_program->bind();
	Ground_program->setUniformValue("projection", parameter::projection);
	Ground_program->setUniformValue("view", parameter::view);
	for (int i = 0; i < 100; i++) {
		Ground_model.setToIdentity();
		Ground_model.translate(0, 0, i);
		for (int j = 0; j < 100; j++) {
			Ground_program->setUniformValue("model", Ground_model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			Ground_model.translate(1, 0, 0);

		}
	}
	Ground_program->release();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	Ground_vbo.release();
	Ground_vao.release();
}

void m_qopenglwidget::initPng()
{
	statusPng = square_tex(QString::fromStdString(parameter::path) + "data/status.png",3,4);
	statusPng.Topping = true;
	statusPng.alpha = 0.8;
	statusPng.bind();

}

void m_qopenglwidget::drawPng()
{
	QPoint p;
	p.setX(0.85 * this->width());
	p.setY(0.2 * this->height());

	QVector3D worldPosition = QVector3D(worldPositionFromMousePosition(p));

	//计算视线的直线方向向量
	QVector3D directionVector = parameter::viewDirection + parameter::cameraDirection - worldPosition;

	//观察平面法向量
	QVector3D planeNormal = QVector3D(0, 0, 1);
	QMatrix4x4 m;
	m.rotate(yaw * 180 / PI, 0, 1, 0);
	m.rotate(-pitch * 180 / PI, 1, 0, 0);
	planeNormal = m * planeNormal;
	QVector3D directionVector_n = parameter::cameraDirection;
	directionVector_n.normalize();
	directionVector_n *= parameter::r - 10.0;
	float planeConstant = -QVector3D::dotProduct(planeNormal, parameter::viewDirection+ directionVector_n);

	//求解参数方程的参数t
	float t = (-planeConstant - QVector3D::dotProduct(worldPosition, planeNormal)) / QVector3D::dotProduct(directionVector, planeNormal);

	QMatrix4x4 mm;
	mm.translate(worldPosition + t * directionVector);
	
	mm.rotate(yaw * 180 / PI, 0, 1, 0);
	mm.rotate(-pitch * 180 / PI, 1, 0, 0);
	
	statusPng.meshModel.setToIdentity();
	statusPng.meshModel.rotate(90, 1, 0, 0);
	statusPng.draw(mm);

}

void m_qopenglwidget::initcompositeCube(const meshNode * node)
{
	makeCurrent();

	if (compositeCube) {
		delete compositeCube;
		compositeCube = NULL;
		parameter::isdrawcompositeCube = false;
	}

	if (node && node->mesh && !node->children.empty()) {
		parameter::isdrawcompositeCube = true;
		compositeCube = new modelCube();
		compositeCube->initdata(node);
	}

}

void m_qopenglwidget::drawcompositeCube()
{
	if (compositeCube) {
		compositeCube->draw();
	}
}

void m_qopenglwidget::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(61.0 / 255.0, 61.0 / 255.0, 61.0 / 255.0, 1.0);
	initSkybox();
	initLight();
	initGround();
	initAxis();
	initPng();
	initTransAxis();
}

void m_qopenglwidget::resizeGL(int w, int h)
{

}

void m_qopenglwidget::paintGL()
{
	parameter::view.setToIdentity();
	parameter::view.lookAt(parameter::viewDirection+parameter::cameraDirection, parameter::viewDirection, parameter::cameraUp);
	
	parameter::projection.setToIdentity();
	parameter::projection.perspective(parameter::fov, (float)width() / (float)height(), parameter::near_, parameter::far_);

	//是否绘制光源点
	if (parameter::isdrawLight) {
		drawLight();
	}
	
	//画天空盒
	if (parameter::isdrawSky) {
		drawSkybox();
	}

	//画地板
	if (parameter::isdrawGround) {
		drawGround();
	}

	//画坐标轴
	drawAxis();

	//画三维模型
	Scene->drawMesh();

	//画复合模型外接框
	if (parameter::isdrawcompositeCube) {
		drawcompositeCube();
	}
	
	//绘制状态面板
	if (parameter::isdrawStatuspng) {
		drawPng();
	}

	drawTransAxis();

	if (selectframe) {
		drawsf();
	}

}

void m_qopenglwidget::inittransformButtons()
{
	selectbtn = new mButton(QString::fromStdString(parameter::path) + "data/icon/select1.png", QString::fromStdString(parameter::path) + "data/icon/select2.png",0);
	transformbtns.push_back(selectbtn);
	scalebtn = new mButton(QString::fromStdString(parameter::path) + "data/icon/scale1.png", QString::fromStdString(parameter::path) + "data/icon/scale2.png",1);
	transformbtns.push_back(scalebtn);
	rotatebtn = new mButton(QString::fromStdString(parameter::path) + "data/icon/rotate1.png", QString::fromStdString(parameter::path) + "data/icon/rotate2.png",2);
	transformbtns.push_back(rotatebtn);
	movebtn = new mButton(QString::fromStdString(parameter::path) + "data/icon/move1.png", QString::fromStdString(parameter::path) + "data/icon/move2.png",3);
	transformbtns.push_back(movebtn);


	// 创建布局并放置按钮
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(selectbtn);
	layout->addWidget(scalebtn);
	layout->addWidget(rotatebtn);
	layout->addWidget(movebtn);
	layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setLayout(layout);

	connect(selectbtn,&mButton::isclick, this, &m_qopenglwidget::handlebtn);
	connect(scalebtn, &mButton::isclick, this, &m_qopenglwidget::handlebtn);
	connect(rotatebtn, &mButton::isclick, this, &m_qopenglwidget::handlebtn);
	connect(movebtn, &mButton::isclick, this, &m_qopenglwidget::handlebtn);

	//默认为选择操作
	selectbtn->setIcon2();
	transformItem = 0;
}

void m_qopenglwidget::initTransAxis()
{
	mAxis.init();
	rAxis.init();
	sAxis.init();

	transAxisMap[TRANS_MOVE] = &mAxis;
	transAxisMap[TRANS_ROTATE] = &rAxis;
	transAxisMap[TRANS_SCALE] = &sAxis;
}

void m_qopenglwidget::drawTransAxis()
{
	for (auto it = transAxisMap.begin(); it != transAxisMap.end(); it++) {
		if (it->first == transformItem && it->second->isDraw) {
			it->second->draw();
		}
	}
}

void m_qopenglwidget::undrawallTransAxis()
{
	for (auto it = transAxisMap.begin(); it != transAxisMap.end(); it++) {
		it->second->isDraw = false;
	}
}

void m_qopenglwidget::initsf()
{
	makeCurrent();

	if (selectframe) {
		delete selectframe;
		selectframe = NULL;
	}

	v2.setX(v3.x());
	v2.setY(v1.y());
	v4.setX(v1.x());
	v4.setY(v3.y());
	selectframe = new SelectFrame(getTargetPosition(worldPositionFromMousePosition(v1)),
		getTargetPosition(worldPositionFromMousePosition(v2)), 
		getTargetPosition(worldPositionFromMousePosition(v3)), 
		getTargetPosition(worldPositionFromMousePosition(v4)));
	selectframe->init();
}

void m_qopenglwidget::drawsf()
{
	if (selectframe) {
		selectframe->draw();
	}
}

void m_qopenglwidget::handlebtn(int index) {
	transformItem = index;
	undrawallTransAxis();
	for (int i = 0; i < transformbtns.size(); i++) {
		if (i != index) {
			transformbtns[i]->setIcon1();
		}
	}
	if (index != 0 && transnode) {
		transAxisMap[index]->base.setToIdentity();
		transAxisMap[index]->base.translate(transnode->mesh->model * transnode->mesh->boxCenter);
		transAxisMap[index]->isDraw = true;
	}
	update();
}