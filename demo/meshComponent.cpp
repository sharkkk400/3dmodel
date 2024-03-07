#include "meshComponent.h"

meshComponent::meshComponent()
{
	color = QVector3D(1, 1, 1);
	alpha = 1.0;
	Topping = false;
	drawitem = DRAW_POLINGON;
	linewidth = 1.0f;
}

meshComponent::~meshComponent()
{
}

void meshComponent::getwpsboxcoord()
{
	boxCoord.clear();
	for (int i = 0; i < Vertex.size(); i++) {
		QVector4D worldcoord = model * QVector4D(Vertex[i], 1.0f);
		if (boxCoord.size() < 6) {
			boxCoord.resize(6);
			boxCoord[0] = worldcoord[0];
			boxCoord[1] = worldcoord[1];
			boxCoord[2] = worldcoord[2];
			boxCoord[3] = worldcoord[0];
			boxCoord[4] = worldcoord[1];
			boxCoord[5] = worldcoord[2];
		}
		else {
			if (worldcoord[0] > boxCoord[0]) {
				boxCoord[0] = worldcoord[0];
			}
			if (worldcoord[1] > boxCoord[1]) {
				boxCoord[1] = worldcoord[1];
			}
			if (worldcoord[2] > boxCoord[2]) {
				boxCoord[2] = worldcoord[2];
			}
			if (worldcoord[0] < boxCoord[3]) {
				boxCoord[3] = worldcoord[0];
			}
			if (worldcoord[1] < boxCoord[4]) {
				boxCoord[4] = worldcoord[1];
			}
			if (worldcoord[2] < boxCoord[5]) {
				boxCoord[5] = worldcoord[2];
			}
		}
	}
}

bool meshComponent::checkSelect(QVector4D worldPosition)
{
	if (worldPosition.x() <= boxCoord[0] &&
		worldPosition.x() >= boxCoord[3] &&
		worldPosition.y() <= boxCoord[1] &&
		worldPosition.y() >= boxCoord[4] &&
		worldPosition.z() <= boxCoord[2] &&
		worldPosition.z() >= boxCoord[5]) {
		return true;
	}
	return false;
}

bool meshComponent::checkSelect(QVector2D viewportXY)
{
	if (drawitem == DRAW_POLINGON) {
		for (int i = 0; i < ScreenVertex.size(); i++) {
			QVector2D v = ScreenVertex[i] - viewportXY;
			if (v.distanceToPoint(QVector2D(0, 0)) < 0.01) {
				return true;
			}
		}
	}
	if (drawitem == DRAW_LINE) {
		for (int i = 0; i < ScreenVertex.size(); i++) {
			QVector2D v = ScreenVertex[i] - viewportXY;
			if (v.distanceToPoint(QVector2D(0, 0)) < 0.01) {
				return true;
			}
		}
	}
	
	return false;
	
}

void meshComponent::updatevieportXY()
{
	QVector4D temp = parameter::projection*parameter::view*model*QVector4D(Vertex[0], 1.0f);
	viewportXmax = temp.x() / temp.w();
	viewportYmax = temp.y() / temp.w();
	viewportXmin = temp.x() / temp.w();
	viewportYmin = temp.y() / temp.w();

	for (int i = 0; i < Vertex.size();i++) {
		QVector4D v = parameter::projection*parameter::view*model*QVector4D(Vertex[i], 1.0f);
		ScreenVertex.push_back(QVector2D(v.x() / v.w(), v.y() / v.w()));
		if (v.x() / temp.w() > viewportXmax) {
			viewportXmax = v.x() / v.w();
		}
		if (v.x() / temp.w() < viewportXmin) {
			viewportXmin = v.x() / v.w();
		}
		if (v.y() / temp.w() > viewportYmax) {
			viewportYmax = v.y() / v.w();
		}
		if (v.y() / temp.w() < viewportYmin) {
			viewportXmin = v.y() / v.w();
		}
	}
}

void meshComponent::bind()
{
	initializeOpenGLFunctions();

	// 创建顶点数组对象
	glGenVertexArrays(1, &vao); // 分配1个顶点数组对象
	glBindVertexArray(vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象 这里填NULL 先不传数据
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(QVector3D), NULL, GL_STATIC_DRAW);

	// 传位置
	glBufferSubData(GL_ARRAY_BUFFER, 0, Vertex.size() * sizeof(QVector3D), Vertex.data());
	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	// 传索引到 ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	program = new QOpenGLShaderProgram();
	program->bind();
	program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	out vec3 ourColor;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform vec3 objectColor;
	uniform bool topping;
	void main()
	{
		gl_Position = projection *view *model*vec4(aPos, 1.0);
		if(topping){gl_Position.z = 0;}
		ourColor = objectColor;
	})");
	program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	uniform float alpha;
	void main()
	{
		gl_FragColor = vec4(ourColor, alpha);
	})");
	program->link();

}

void meshComponent::draw(QMatrix4x4 base)
{
	baseModel = base;
	model = baseModel * meshModel;

	updatevieportXY();

	glUseProgram(program->programId());
	program->setUniformValue("projection", parameter::projection);
	program->setUniformValue("view", parameter::view);
	glBindVertexArray(vao);
	program->setUniformValue("model", model);
	program->setUniformValue("objectColor", color);
	program->setUniformValue("alpha", alpha);
	program->setUniformValue("topping", Topping);
	switch (drawitem)
	{
	case DRAW_POLINGON:
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		break;
	case DRAW_LINE:
		glLineWidth(linewidth);
		glDrawElements(GL_LINES, index.size(), GL_UNSIGNED_INT, 0);
		break;
	default:
		break;
	}
	
	glBindVertexArray(0);
}
