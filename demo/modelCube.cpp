#include "modelCube.h"

modelCube::modelCube()
{
	initializeOpenGLFunctions();
}

modelCube::~modelCube()
{
}

void modelCube::initdata(const meshNode * node)
{
	coords.clear();
	edges.clear();

	loadcoords(node);
	coords2edges();

	binddata();
}

void modelCube::loadcoords(const meshNode * node)
{
	if (node->mesh) {
		for (auto v : node->mesh->vertexPosition) {
			QVector4D worldcoord = node->mesh->model * QVector4D(v, 1.0f);
			if (coords.size() < 6) {
				coords.resize(6);
				coords[0] = worldcoord[0];
				coords[1] = worldcoord[1];
				coords[2] = worldcoord[2];
				coords[3] = worldcoord[0];
				coords[4] = worldcoord[1];
				coords[5] = worldcoord[2];
			}
			else {
				if (worldcoord[0] > coords[0]) {
					coords[0] = worldcoord[0];
				}
				if (worldcoord[1] > coords[1]) {
					coords[1] = worldcoord[1];
				}
				if (worldcoord[2] > coords[2]) {
					coords[2] = worldcoord[2];
				}
				if (worldcoord[0] < coords[3]) {
					coords[3] = worldcoord[0];
				}
				if (worldcoord[1] < coords[4]) {
					coords[4] = worldcoord[1];
				}
				if (worldcoord[2] < coords[5]) {
					coords[5] = worldcoord[2];
				}
			}
		}
	}

	for (auto child : node->children) {
		loadcoords(child);
	}
}

void modelCube::initdata(std::vector<float>& coordinates)
{
	coords.clear();
	edges.clear();

	coords2edges(coordinates);

	binddata();
}

void modelCube::coords2edges(std::vector<float> &coordinates)
{
	edges.push_back(QVector3D(coordinates[0], coordinates[1], coordinates[2]));
	edges.push_back(QVector3D(coordinates[0], coordinates[4], coordinates[2]));

	edges.push_back(QVector3D(coordinates[0], coordinates[4], coordinates[2]));
	edges.push_back(QVector3D(coordinates[0], coordinates[4], coordinates[5]));

	edges.push_back(QVector3D(coordinates[0], coordinates[4], coordinates[5]));
	edges.push_back(QVector3D(coordinates[0], coordinates[1], coordinates[5]));

	edges.push_back(QVector3D(coordinates[0], coordinates[1], coordinates[5]));
	edges.push_back(QVector3D(coordinates[0], coordinates[1], coordinates[2]));

	edges.push_back(QVector3D(coordinates[3], coordinates[1], coordinates[2]));
	edges.push_back(QVector3D(coordinates[3], coordinates[4], coordinates[2]));

	edges.push_back(QVector3D(coordinates[3], coordinates[4], coordinates[2]));
	edges.push_back(QVector3D(coordinates[3], coordinates[4], coordinates[5]));

	edges.push_back(QVector3D(coordinates[3], coordinates[4], coordinates[5]));
	edges.push_back(QVector3D(coordinates[3], coordinates[1], coordinates[5]));

	edges.push_back(QVector3D(coordinates[3], coordinates[1], coordinates[5]));
	edges.push_back(QVector3D(coordinates[3], coordinates[1], coordinates[2]));

	edges.push_back(QVector3D(coordinates[0], coordinates[1], coordinates[2]));
	edges.push_back(QVector3D(coordinates[3], coordinates[1], coordinates[2]));

	edges.push_back(QVector3D(coordinates[0], coordinates[4], coordinates[2]));
	edges.push_back(QVector3D(coordinates[3], coordinates[4], coordinates[2]));

	edges.push_back(QVector3D(coordinates[0], coordinates[4], coordinates[5]));
	edges.push_back(QVector3D(coordinates[3], coordinates[4], coordinates[5]));

	edges.push_back(QVector3D(coordinates[0], coordinates[1], coordinates[5]));
	edges.push_back(QVector3D(coordinates[3], coordinates[1], coordinates[5]));
}

void modelCube::binddata()
{
	// 创建顶点数组对象
	glGenVertexArrays(1, &cube_vao); // 分配1个顶点数组对象
	glBindVertexArray(cube_vao);  	// 绑定顶点数组对象

	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, edges.size() * sizeof(QVector3D), edges.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	glBindVertexArray(0);

	cube_program = new QOpenGLShaderProgram();
	cube_program->bind();
	cube_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	out vec3 ourColor;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	uniform vec3 color;
	void main()
	{
		gl_Position = projection *view *model *vec4(aPos, 1.0);
		ourColor = color;
	})");
	cube_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	void main()
	{
		gl_FragColor = vec4(ourColor, 1.0);
	})");
	cube_program->link();

}

void modelCube::initdata(const QList<Mesh*> &meshes)
{
	coords.clear();
	edges.clear();

	loadcoords(meshes);
	coords2edges();

	binddata();
}

void modelCube::loadcoords(const QList<Mesh*>& meshes)
{
	for (int i = 0; i < meshes.size(); i++) {
		QMatrix4x4 m = meshes[i]->model;
		for (int j = 0; j < meshes[i]->vertexPosition.size(); j++) {

			//转换为世界坐标
			QVector4D worldcoord = m * QVector4D(meshes[i]->vertexPosition[j], 1.0f);

			if (coords.size() < 6) {
				coords.resize(6);
				coords[0] = worldcoord[0];
				coords[1] = worldcoord[1];
				coords[2] = worldcoord[2];
				coords[3] = worldcoord[0];
				coords[4] = worldcoord[1];
				coords[5] = worldcoord[2];
			}
			else {
				if (worldcoord[0] > coords[0]) {
					coords[0] = worldcoord[0];
				}
				if (worldcoord[1] > coords[1]) {
					coords[1] = worldcoord[1];
				}
				if (worldcoord[2] > coords[2]) {
					coords[2] = worldcoord[2];
				}
				if (worldcoord[0] < coords[3]) {
					coords[3] = worldcoord[0];
				}
				if (worldcoord[1] < coords[4]) {
					coords[4] = worldcoord[1];
				}
				if (worldcoord[2] < coords[5]) {
					coords[5] = worldcoord[2];
				}
			}
		}
	}

	center = QVector3D((coords[0] + coords[3]) / 2, (coords[1] + coords[4]) / 2, (coords[2] + coords[5]) / 2);
}

void modelCube::coords2edges()
{
	center = QVector3D((coords[0] + coords[3]) / 2, (coords[1] + coords[4]) / 2, (coords[2] + coords[5]) / 2);

	edges.push_back(QVector3D(coords[0], coords[1], coords[2]));
	edges.push_back(QVector3D(coords[0], coords[4], coords[2]));

	edges.push_back(QVector3D(coords[0], coords[4], coords[2]));
	edges.push_back(QVector3D(coords[0], coords[4], coords[5]));

	edges.push_back(QVector3D(coords[0], coords[4], coords[5]));
	edges.push_back(QVector3D(coords[0], coords[1], coords[5]));

	edges.push_back(QVector3D(coords[0], coords[1], coords[5]));
	edges.push_back(QVector3D(coords[0], coords[1], coords[2]));

	edges.push_back(QVector3D(coords[3], coords[1], coords[2]));
	edges.push_back(QVector3D(coords[3], coords[4], coords[2]));

	edges.push_back(QVector3D(coords[3], coords[4], coords[2]));
	edges.push_back(QVector3D(coords[3], coords[4], coords[5]));

	edges.push_back(QVector3D(coords[3], coords[4], coords[5]));
	edges.push_back(QVector3D(coords[3], coords[1], coords[5]));

	edges.push_back(QVector3D(coords[3], coords[1], coords[5]));
	edges.push_back(QVector3D(coords[3], coords[1], coords[2]));

	edges.push_back(QVector3D(coords[0], coords[1], coords[2]));
	edges.push_back(QVector3D(coords[3], coords[1], coords[2]));

	edges.push_back(QVector3D(coords[0], coords[4], coords[2]));
	edges.push_back(QVector3D(coords[3], coords[4], coords[2]));

	edges.push_back(QVector3D(coords[0], coords[4], coords[5]));
	edges.push_back(QVector3D(coords[3], coords[4], coords[5]));

	edges.push_back(QVector3D(coords[0], coords[1], coords[5]));
	edges.push_back(QVector3D(coords[3], coords[1], coords[5]));
}

void modelCube::draw(QMatrix4x4 model, QVector3D color)
{
	glUseProgram(cube_program->programId());
	glBindVertexArray(cube_vao);
	//传变换矩阵
	cube_program->setUniformValue("projection", parameter::projection);
	cube_program->setUniformValue("view", parameter::view);
	cube_program->setUniformValue("model", model);
	cube_program->setUniformValue("color", color);
	glLineWidth(1.0f);
	for (int i = 0; i < 12; i++) {
		glDrawArrays(GL_LINES, 2 * i, 2);
	}
	glBindVertexArray(0);
}
