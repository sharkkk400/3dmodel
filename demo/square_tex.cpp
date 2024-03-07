#include "square_tex.h"

square_tex::square_tex(QString pngPath, float x ,float z)
{
	this->pngPath = pngPath;

	Vertex.clear();
	Vertex.push_back(QVector3D(x / 2.0, 0, z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, 0, z / 2.0));
	Vertex.push_back(QVector3D(x / 2.0, 0, -z / 2.0));
	Vertex.push_back(QVector3D(-x / 2.0, 0, -z / 2.0));

	index.clear();
	index.push_back(0);
	index.push_back(2);
	index.push_back(3);
	index.push_back(0);
	index.push_back(3);
	index.push_back(1);

	texcoord.clear();
	texcoord.push_back(QVector2D(1, 0));
	texcoord.push_back(QVector2D(0, 0));
	texcoord.push_back(QVector2D(1, 1));
	texcoord.push_back(QVector2D(0, 1));
}

square_tex::~square_tex()
{
}

void square_tex::bind()
{
	initializeOpenGLFunctions();

	// 创建顶点数组对象
	glGenVertexArrays(1, &vao); // 分配1个顶点数组对象
	glBindVertexArray(vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象 这里填NULL 先不传数据
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, Vertex.size() * sizeof(QVector3D) + texcoord.size() * sizeof(QVector2D), NULL, GL_STATIC_DRAW);

	// 传位置
	GLuint offset_position = 0;
	GLuint size_position = Vertex.size() * sizeof(QVector3D);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_position, Vertex.data());
	glEnableVertexAttribArray(0);   // 着色器中 (layout = 0) 表示顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));

	// 传纹理坐标
	GLuint offset_texcoord = size_position;
	GLuint size_texcoord = texcoord.size() * sizeof(QVector2D);
	glBufferSubData(GL_ARRAY_BUFFER, offset_texcoord, size_texcoord, texcoord.data());
	glEnableVertexAttribArray(1);   // 着色器中 (layout = 1) 表示纹理坐标
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(offset_texcoord));

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
	layout (location = 1) in vec2 aTextureCoord;
	out vec2 textureCoord;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 model;
	uniform bool topping;
	void main()
	{
		gl_Position = projection *view *model*vec4(aPos, 1.0);
		if(topping){gl_Position.z = 0;}
		textureCoord = aTextureCoord;
	})");
	program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	out vec4 fragColor;
	in vec2 textureCoord;
	uniform sampler2D textureImg;
	uniform float alpha;
	void main()
	{
		fragColor = vec4(texture(textureImg, textureCoord).rbg,alpha);
	})");
	program->link();

	//垂直镜像mirrored
	Texture = new QOpenGLTexture(QImage(pngPath).mirrored());
	Texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
	Texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
	Texture->setMinificationFilter(QOpenGLTexture::Linear);
	Texture->setMagnificationFilter(QOpenGLTexture::Linear);

	program->setUniformValue("textureImg", 0);
}

void square_tex::draw(QMatrix4x4 base)
{
	baseModel = base;
	model = baseModel * meshModel;

	updatevieportXY();

	glUseProgram(program->programId());
	program->setUniformValue("projection", parameter::projection);
	program->setUniformValue("view", parameter::view);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	Texture->bind();
	program->setUniformValue("model", model);
	program->setUniformValue("alpha", alpha);
	program->setUniformValue("topping", Topping);
	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

