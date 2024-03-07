#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::bindData() {
	isbind = true;
	initializeOpenGLFunctions();

	// 创建顶点数组对象
	glGenVertexArrays(1, &vao); // 分配1个顶点数组对象
	glBindVertexArray(vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象 这里填NULL 先不传数据
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		vertexPosition.size() * sizeof(QVector3D) +
		vertexTexcoord.size() * sizeof(QVector2D) +
		vertexNormal.size() * sizeof(QVector3D),
		NULL, GL_STATIC_DRAW);

	// 传位置
	GLuint offset_position = 0;
	GLuint size_position = vertexPosition.size() * sizeof(QVector3D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_position, size_position, vertexPosition.data());
	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_position));
	// 传纹理坐标
	GLuint offset_texcoord = size_position;
	GLuint size_texcoord = vertexTexcoord.size() * sizeof(QVector2D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_texcoord, size_texcoord, vertexTexcoord.data());
	glEnableVertexAttribArray(1);   // 着色器中 (layout = 1) 表示纹理坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_texcoord));
	// 传法线
	GLuint offset_normal = size_position + size_texcoord;
	GLuint size_normal = vertexNormal.size() * sizeof(QVector3D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_normal, size_normal, vertexNormal.data());
	glEnableVertexAttribArray(2);   // 着色器中 (layout = 2) 表示法线
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_normal));

	// 传索引到 ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	//外包立方体

	// 创建顶点数组对象
	glGenVertexArrays(1, &box_vao); // 分配1个顶点数组对象
	glBindVertexArray(box_vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象 这里填NULL 先不传数据
	glGenBuffers(1, &box_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_vbo);
	glBufferData(GL_ARRAY_BUFFER, boxVertex.size() * sizeof(QVector3D), boxVertex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	glBindVertexArray(0);

	//坐标轴
	Axis_mesh.len = lenth;
	Axis_mesh.r = lenth / 50;
	Axis_mesh.h = lenth / 25;
	Axis_mesh.init();

	initShaderProgram();

	initModel();
}

void Mesh::initShaderProgram()
{
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
		in vec2 texcoord;    // 纹理坐标
		in vec3 normal;
		in vec3 world_pos;
		out vec4 fColor;    // 片元输出像素的颜色
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
			fColor = vec4((ambient + diffuse_color + specular_color) * objectColor,1.0);
		})");
	Mesh_program->link();

	MeshBox_program = new QOpenGLShaderProgram();
	MeshBox_program->bind();
	MeshBox_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
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
	MeshBox_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	in vec3 ourColor;
	void main()
	{
		gl_FragColor = vec4(ourColor, 1.0);
	})");
	MeshBox_program->link();
}

void Mesh::draw()
{
	glUseProgram(Mesh_program->programId());
	glBindVertexArray(vao);
	//传变换矩阵
	Mesh_program->setUniformValue("model", model);
	Mesh_program->setUniformValue("projection", parameter::projection);
	Mesh_program->setUniformValue("view", parameter::view);
	Mesh_program->setUniformValue("view_pos", parameter::cameraDirection+parameter::viewDirection);
	Mesh_program->setUniformValue("light_pos", parameter::lightPosition);
	Mesh_program->setUniformValue("ambient_strength", parameter::ambient_strength);
	Mesh_program->setUniformValue("lightColor", parameter::lightColor);
	Mesh_program->setUniformValue("objectColor", objectColor);
	Mesh_program->setUniformValue("Ns", Ns);
	Mesh_program->setUniformValue("u_normal_matrix", model.inverted().transposed());

	//传纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	glUniform1i(glGetUniformLocation(Mesh_program->programId(), "texture"), 0);

	glDrawElements(GL_TRIANGLES, this->index.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (isSelect) {
		glUseProgram(MeshBox_program->programId());
		glBindVertexArray(box_vao);
		//传变换矩阵
		MeshBox_program->setUniformValue("model", model);
		MeshBox_program->setUniformValue("projection", parameter::projection);
		MeshBox_program->setUniformValue("view", parameter::view);
		MeshBox_program->setUniformValue("objectColor", objectColor);
		glLineWidth(2.0f);
		for (int i = 0; i < 12; i++) {
			glDrawArrays(GL_LINES, 2 * i, 2);
		}
		glBindVertexArray(0);

		//绘制局部坐标轴
		{
			QMatrix4x4 base;
			base.setToIdentity();
			base.translate(boxCenter);
			//Axis_mesh.draw(model*base);
		}
		
	}
}

void Mesh::initModel(QMatrix4x4 base, QVector3D parentBox)
{
	baseModel = base;
	meshModel.setToIdentity();
	model = baseModel * meshModel;

	boxCenter_wps = meshModel * boxCenter;

	target = boxCenter;

	target_wps = meshModel * target;

	target_wpsX = boxCenter_wps.x();
	target_wpsY = boxCenter_wps.y();
	target_wpsZ = boxCenter_wps.z();

	getwpsboxcoord();
}

void Mesh::correctModel(QMatrix4x4 base)
{
	baseModel = base;

	model = baseModel * meshModel;

	getwpsboxcoord();
}

void Mesh::getwpsboxcoord()
{
	boxCoord.clear();
	for (int i = 0; i < vertexPosition.size(); i++) {
		QVector4D worldcoord = model * QVector4D(vertexPosition[i],1.0f);
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

bool Mesh::checkSelect(QVector4D worldPosition)
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

bool Mesh::checkSelect(QVector3D linedir)
{
	for (int i = 0; i < vertexPosition.size(); i++) {
		QVector3D worldcoord = QVector3D(model * QVector4D(vertexPosition[i], 1.0f));
		QVector3D PA = worldcoord - parameter::viewDirection-parameter::cameraDirection;
		linedir.normalize();
		QVector3D projection = QVector3D::dotProduct(PA, linedir) * linedir;
		float distance = (PA - projection).length();
		if (distance < 0.01) {
			return true;
		}
	}
	return false;
}

void Mesh::moveModel(QVector4D newwps)
{
	target_wpsX = newwps.x();
	target_wpsY = newwps.y();
	target_wpsZ = newwps.z();
	updateModel();
}

void Mesh::updateModel()
{

	meshModel.setToIdentity();
	meshModel.scale(scaleX, scaleY, scaleZ);

	meshModel.rotate(rotateX, QVector3D(1, 0, 0));
	meshModel.rotate(rotateY, QVector3D(0, 1, 0));
	meshModel.rotate(rotateZ, QVector3D(0, 0, 1));

	target_wps = QVector3D(target_wpsX, target_wpsY, target_wpsZ);
	target = meshModel.inverted() * target_wps;
	moveVctor = target- boxCenter;
	boxCenter_wps = meshModel * boxCenter;
	meshModel.translate(moveVctor);

	model = baseModel * meshModel;

	getwpsboxcoord();

}
