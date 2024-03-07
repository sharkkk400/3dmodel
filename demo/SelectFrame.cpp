#include "SelectFrame.h"

SelectFrame::SelectFrame(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4)
{
	polygonV.push_back(v1);
	polygonV.push_back(v2);
	polygonV.push_back(v3);
	polygonV.push_back(v1);
	polygonV.push_back(v3);
	polygonV.push_back(v4);

	QVector3D step12 = (v2 - v1) / 60.0;
	for (int i = 0; i < 60; i++) {
		frameV.push_back(v1 + i * step12);
	}

	QVector3D step23 = (v3 - v2) / 60.0;
	for (int i = 0; i < 60; i++) {
		frameV.push_back(v2 + i * step23);
	}

	QVector3D step34 = (v4 - v3) / 60.0;
	for (int i = 0; i < 60; i++) {
		frameV.push_back(v3 + i * step34);
	}

	QVector3D step41 = (v1 - v4) / 60.0;
	for (int i = 0; i < 60; i++) {
		frameV.push_back(v4 + i * step41);
	}

	for (int i = 0; i <= 30;i++) {
		QVector3D start = v1 + i * (v4 - v1) / 30.0;
		for (int j = 0; j <= 30; j++) {
			rayV.push_back(start + j * (v2 - v1) / 30.0);
		}
	}
	
	
}

SelectFrame::~SelectFrame()
{
}

void SelectFrame::init()
{
	//传数据
	initializeOpenGLFunctions();

	// 创建顶点数组对象
	glGenVertexArrays(1, &polygon_vao); // 分配1个顶点数组对象
	glBindVertexArray(polygon_vao);  	// 绑定顶点数组对象

	glGenBuffers(1, &polygon_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, polygon_vbo);
	glBufferData(GL_ARRAY_BUFFER, polygonV.size() * sizeof(QVector3D), polygonV.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	glBindVertexArray(0);

	polygon_program = new QOpenGLShaderProgram();
	polygon_program->bind();
	polygon_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	out vec3 ourColor;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
		gl_Position = projection *view * vec4(aPos, 1.0);
		gl_Position.z = 0;
		ourColor = vec3(1.0,1.0,1.0);
	})");
	polygon_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	void main()
	{
		gl_FragColor = vec4(ourColor, 0.2);
	})");
	polygon_program->link();



	// 创建顶点数组对象
	glGenVertexArrays(1, &frame_vao); // 分配1个顶点数组对象
	glBindVertexArray(frame_vao);  	// 绑定顶点数组对象

	glGenBuffers(1, &frame_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, frame_vbo);
	glBufferData(GL_ARRAY_BUFFER, frameV.size() * sizeof(QVector3D), frameV.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	glBindVertexArray(0);

	frame_program = new QOpenGLShaderProgram();
	frame_program->bind();
	frame_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	out vec3 ourColor;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
		gl_Position = projection *view * vec4(aPos, 1.0);
		gl_Position.z = 0;
		ourColor = vec3(1.0,1.0,1.0);
	})");
	frame_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	void main()
	{
		gl_FragColor = vec4(ourColor, 1.0);
	})");
	frame_program->link();
}

void SelectFrame::draw()
{
	glUseProgram(polygon_program->programId());
	polygon_program->setUniformValue("projection", parameter::projection);
	polygon_program->setUniformValue("view", parameter::view);
	glBindVertexArray(polygon_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glUseProgram(frame_program->programId());
	frame_program->setUniformValue("projection", parameter::projection);
	frame_program->setUniformValue("view", parameter::view);
	glBindVertexArray(frame_vao);
	glLineWidth(3.0f);
	glDrawArrays(GL_LINES, 0, frameV.size());
	glBindVertexArray(0);
}

void SelectFrame::checkin(std::map<QString, meshNode*>& nodeMap, std::set<meshNode*> &selectedNodes)
{
	for (auto it = nodeMap.begin(); it != nodeMap.end(); it++) {
		for (auto ray : rayV) {
			if (it->second->mesh->checkSelect(ray - parameter::cameraDirection - parameter::viewDirection)) {
				selectedNodes.insert(it->second);
				break;
			}
		}
	}
}
