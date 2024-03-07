#include "Axis.h"

Axis::Axis(float len, float r, float h, int x)
{
	this->len = len;
	this->r = r;
	this->h = h;
	this->x = x;
}

Axis::~Axis()
{
}

void Axis::init()
{
	//Բ׶(�� x+2 ����)
	for (int i = 0; i < x; i++) {
		Vertex.push_back(QVector3D(r*cos(i*2*PI / float(x)), -h / 2.0, -r * sin(i * 2*PI / float(x))));
	}
	Vertex.push_back(QVector3D(0, -h / 2.0, 0));
	Vertex.push_back(QVector3D(0, h / 2.0, 0));

	//Բ������ 2x+1 ���㣩
	for (int i = 0; i < x; i++) {
		Vertex.push_back(QVector3D(r / 2.0*cos(i*2*PI / float(x)), -len / 2.0, -r / 2.0*sin(i*2*PI / float(x))));
		Vertex.push_back(QVector3D(r / 2.0*cos(i*2*PI / float(x)),  len / 2.0, -r / 2.0*sin(i*2*PI / float(x))));
	}
	Vertex.push_back(QVector3D(0, -len / 2.0, 0));


	for (int i = 0; i < x; i++) {
		index.push_back(x);
		index.push_back(i);
		index.push_back(i + 1 < x ? i + 1 : 0);
		index.push_back(x + 1);
		index.push_back(i + 1 < x ? i + 1 : 0);
		index.push_back(i);
	}

	for (int i = 0; i < x; i++) {
		index.push_back(3 * x + 2);
		index.push_back(x + 2 + 2 * i);
		index.push_back(i!=x-1 ? (x+4+2*i):(x+2));
	}

	for (int i = 0; i < x; i++) {
		index.push_back(x + 2 + i * 2);
		index.push_back(i != x - 1 ? x + 5 + i * 2 : x + 3);
		index.push_back(i != x - 1 ? x + 4 + i * 2 : x + 2);
		index.push_back(x + 2 + i * 2);
		index.push_back(x + 3 + i * 2);
		index.push_back(i != x - 1 ? x + 5 + i * 2 : x + 3);
	}

	bind();
}

void Axis::bind()
{
	initializeOpenGLFunctions();

	// ���������������
	glGenVertexArrays(1, &cone_vao); // ����1�������������
	glBindVertexArray(cone_vao);  	// �󶨶����������

	// ��������ʼ�����㻺����� ������NULL �Ȳ�������
	glGenBuffers(1, &cone_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
	glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(QVector3D), NULL, GL_STATIC_DRAW);

	// ��λ��
	glBufferSubData(GL_ARRAY_BUFFER, 0, Vertex.size() * sizeof(QVector3D), Vertex.data());
	glEnableVertexAttribArray(0);   // ��ɫ���� (layout = 0) ��ʾ����λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	// �������� ebo
	glGenBuffers(1, &cone_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	cone_program = new QOpenGLShaderProgram();
	cone_program->bind();
	cone_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	out vec3 ourColor;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform vec3 objectColor;
	void main()
	{
		gl_Position = projection *view *model*vec4(aPos, 1.0);
		ourColor = objectColor;
	})");
	cone_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	void main()
	{
		gl_FragColor = vec4(ourColor, 1.0);
	})");
	cone_program->link();

}

void Axis::draw(QMatrix4x4 base, QMatrix4x4 proj, QMatrix4x4 view)
{
	glUseProgram(cone_program->programId());
	cone_program->setUniformValue("projection", proj);
	cone_program->setUniformValue("view", view);

	//X��
	{
		m.setToIdentity();
		m.translate(len, 0, 0);
		m.rotate(270, 0, 0, 1);
		glBindVertexArray(cone_vao);
		cone_program->setUniformValue("model", base*m);
		cone_program->setUniformValue("objectColor", QVector3D(1.0, 0.0, 0.0));
		glDrawElements(GL_TRIANGLES, 6 * x, GL_UNSIGNED_INT, 0);
		m.setToIdentity();
		m.translate(len / 2.0, 0, 0);
		m.rotate(270, 0, 0, 1);
		cone_program->setUniformValue("model", base*m);
		glDrawElements(GL_TRIANGLES, 9 * x, GL_UNSIGNED_INT, (GLvoid*)(6 * x * sizeof(int)));
		glBindVertexArray(0);
	}

	
	//Y��
	{
		m.setToIdentity();
		m.translate(0, len, 0);
		glBindVertexArray(cone_vao);
		cone_program->setUniformValue("model", base*m);
		cone_program->setUniformValue("objectColor", QVector3D(0.0, 1.0, 0.0));
		glDrawElements(GL_TRIANGLES, 6*x, GL_UNSIGNED_INT, 0);
		m.setToIdentity();
		m.translate(0, len / 2.0, 0);
		cone_program->setUniformValue("model", base*m);
		glDrawElements(GL_TRIANGLES, 9 * x, GL_UNSIGNED_INT, (GLvoid*)(6 * x * sizeof(int)));
		glBindVertexArray(0);
	}
	
	//Z��
	{
		m.setToIdentity();
		m.translate(0, 0, len);
		m.rotate(90, 1, 0, 0);
		glBindVertexArray(cone_vao);
		cone_program->setUniformValue("model", base*m);
		cone_program->setUniformValue("objectColor", QVector3D(0.0, 0.0, 1.0));
		glDrawElements(GL_TRIANGLES, 6*x , GL_UNSIGNED_INT, 0);
		m.setToIdentity();
		m.translate(0, 0, len / 2.0);
		m.rotate(90, 1, 0, 0);
		cone_program->setUniformValue("model", base*m);
		glDrawElements(GL_TRIANGLES, 9 * x, GL_UNSIGNED_INT, (GLvoid*)(6 * x * sizeof(int)));
		glBindVertexArray(0);
	}
	
}
