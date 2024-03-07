#include "detail.h"

detail::detail( Mesh* m, QWidget* parent)
{
	this->m = m;
}

detail::~detail()
{
}

void detail::mouseMoveEvent(QMouseEvent * event)
{
	if (changeview_) {
		//���������������δ�ɿ��������ӽǷ����仯
		float offset_x = event->x() - last_x;
		float offset_y = event->y() - last_y;
		if (offset_x < 0) {  //���������offset_x>0
			yaw += sensitivity * 1 * 3.14 / 180;
		}
		else if (offset_x > 0) {
			yaw -= sensitivity * 1 * 3.14 / 180;
		}
		if (offset_y > 0) {  //���������offest_y<0
			pitch += sensitivity * 1 * 3.14 / 180;
		}
		else if (offset_y < 0) {
			pitch -= sensitivity * 1 * 3.14 / 180;
		}
		if (yaw > 6.28 || yaw < -6.28)
			yaw = 0;
		if (pitch > 3.14 / 2) {
			pitch = 3.13 / 2;
		}
		if (pitch < -3.14 / 2) {
			pitch = -3.13 / 2;
		}

		float x_change = r * sin(yaw)*cos(pitch);
		float y_change = r * sin(pitch);
		float z_change = r * cos(yaw)*cos(pitch);

		cameraDirection.setX(x_change);
		cameraDirection.setY(y_change);
		cameraDirection.setZ(z_change);
		cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

	}
	
	this->repaint();
	last_x = event->x();
	last_y = event->y();
}

void detail::mousePressEvent(QMouseEvent * event)
{
	last_x = event->x();
	last_y = event->y();
	if (event->button() == Qt::RightButton) {  //��������������ı�����ӽ�
		changeview_ = true;
	}
	
}

void detail::mouseReleaseEvent(QMouseEvent * event)
{
	changeview_ = false;
	moveModel = false;
}

void detail::wheelEvent(QWheelEvent * event)
{
	if (event->delta() > 0) {
		r = r - cameraSpeed;
	}
	else {
		r = r + cameraSpeed;
	}
	float x_change = r * sin(yaw)*cos(pitch);
	float y_change = r * sin(pitch);
	float z_change = r * cos(yaw)*cos(pitch);

	cameraDirection.setX(x_change);
	cameraDirection.setY(y_change);
	cameraDirection.setZ(z_change);

	this->repaint();
}

void detail::initAxis()
{
	Axis_model.setToIdentity();

	Axis_program = new QOpenGLShaderProgram();
	Axis_program->bind();
	//�����ɫ��
	{
		//��Ӷ�����ɫ��
		Axis_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	out vec3 ourColor;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	void main()
	{
		gl_Position = projection *view *model*vec4(aPos, 1.0);
		ourColor = aColor;
	})");
		//���Ƭ����ɫ��
		Axis_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	void main()
	{
		gl_FragColor = vec4(ourColor, 1.0);
	})");
	}
	Axis_program->link();
	Axis_vbo.create();
	Axis_vbo.bind();
	Axis_vbo.allocate(AxisV.data(), AxisV.size() * sizeof(AxisV));

	//�򶥵���ɫ���������ж���Ϊ"aPos"�ı������������
	Axis_program->setAttributeBuffer(Axis_program->attributeLocation("aPos"), GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
	Axis_program->enableAttributeArray(Axis_program->attributeLocation("aPos"));

	//�򶥵���ɫ���������ж���Ϊ"aColor"�ı������������
	Axis_program->setAttributeBuffer(Axis_program->attributeLocation("aColor"), GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
	Axis_program->enableAttributeArray(Axis_program->attributeLocation("aColor"));
	Axis_program->release();//������
	Axis_vbo.release();
}

void detail::drawAxis()
{
	Axis_vbo.bind();
	Axis_program->bind();
	Axis_program->setUniformValue("projection", projection);
	Axis_program->setUniformValue("view", view);
	Axis_program->setUniformValue("model", Axis_model);

	glLineWidth(6.0f);
	glDrawArrays(GL_LINES, 0, 2);//���ƻ�����ϵ
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);
	Axis_program->release();//������
	Axis_vbo.release();
}

void detail::initmesh()
{
	initializeOpenGLFunctions();

	// ���������������
	glGenVertexArrays(1, &vao); // ����1�������������
	glBindVertexArray(vao);  	// �󶨶����������

	// ��������ʼ�����㻺����� ������NULL �Ȳ�������
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		m->vertexPosition.size() * sizeof(QVector3D) +
		m->vertexTexcoord.size() * sizeof(QVector2D) +
		m->vertexNormal.size() * sizeof(QVector3D),
		NULL, GL_STATIC_DRAW);

	// ��λ��
	GLuint offset_position = 0;
	GLuint size_position = m->vertexPosition.size() * sizeof(QVector3D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_position, size_position, m->vertexPosition.data());
	glEnableVertexAttribArray(0);   // ��ɫ���� (layout = 0) ��ʾ����λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_position));
	// ����������
	GLuint offset_texcoord = size_position;
	GLuint size_texcoord = m->vertexTexcoord.size() * sizeof(QVector2D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_texcoord, size_texcoord, m->vertexTexcoord.data());
	glEnableVertexAttribArray(1);   // ��ɫ���� (layout = 1) ��ʾ��������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_texcoord));
	// ������
	GLuint offset_normal = size_position + size_texcoord;
	GLuint size_normal = m->vertexNormal.size() * sizeof(QVector3D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_normal, size_normal, m->vertexNormal.data());
	glEnableVertexAttribArray(2);   // ��ɫ���� (layout = 2) ��ʾ����
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_normal));

	// �������� ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->index.size() * sizeof(GLuint), m->index.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);


	Mesh_program = new QOpenGLShaderProgram();
	Mesh_program->bind();
	Mesh_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
		#version 330 core
		layout(location = 0) in vec3 vPosition;
		layout(location = 1) in vec2 vTexcoord;
		layout(location = 2) in vec3 vNormal;
		out vec2 texcoord;
		out vec3 normal;
		out vec3 world_pos;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		uniform mat4 u_normal_matrix;
		void main()
		{
			gl_Position = projection * view * model * vec4(vPosition, 1.0);
			texcoord = vTexcoord;
			normal = normalize(mat3(u_normal_matrix)*vNormal);
			world_pos = vec3(model * vec4(vPosition, 1.0));
		})");
	Mesh_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
		#version 330 core
		in vec2 texcoord;    // ��������
		in vec3 normal;
		in vec3 world_pos;
		out vec4 fColor;    // ƬԪ������ص���ɫ
		uniform vec3 view_pos;
		uniform vec3 light_pos;
		uniform float ambient_strength;
		uniform vec3 lightColor;
		uniform vec3 objectColor;
		uniform float Ns;
		void main()
		{
			vec3 ambient = ambient_strength * lightColor;
			vec3 light_dir = normalize(light_pos - world_pos);
			vec3 view_dir = normalize(view_pos - world_pos);
			vec3 reflect_dir = reflect(-light_dir, normal);
			float specular_strength = 0.5;
			vec3 specular_color = specular_strength * lightColor * pow(max(dot(view_dir, reflect_dir), 0.0), Ns);
			vec3 diffuse_color = lightColor * max(0.0, dot(normal, light_dir));
			fColor.rgb = (ambient + diffuse_color + specular_color) * objectColor;
		})");

	Mesh_program->link();
}

void detail::drawmesh()
{
	glUseProgram(Mesh_program->programId());
	glBindVertexArray(vao);
	
	Mesh_program->setUniformValue("model", m->meshModel);
	Mesh_program->setUniformValue("projection", projection);
	Mesh_program->setUniformValue("view", view);
	Mesh_program->setUniformValue("view_pos", cameraDirection);
	Mesh_program->setUniformValue("light_pos", parameter::lightPosition);
	Mesh_program->setUniformValue("ambient_strength", parameter::ambient_strength);
	Mesh_program->setUniformValue("lightColor", parameter::lightColor);
	Mesh_program->setUniformValue("objectColor", QVector3D(0.3451, 0.5647, 0.8824));
	Mesh_program->setUniformValue("Ns", 32);
	Mesh_program->setUniformValue("u_normal_matrix", m->normalModel);

	glDrawElements(GL_TRIANGLES, m->index.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void detail::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	initAxis();
	initmesh();
}

void detail::resizeGL(int w, int h)
{
}

void detail::paintGL()
{	
	view.setToIdentity();
	view.lookAt(cameraDirection, QVector3D(0.0f, 0.0f, 0.0f), cameraUp);
	projection.setToIdentity();
	projection.perspective(fov, (float)width() / (float)height(), near_, far_);
	drawAxis();
	drawmesh();
}
