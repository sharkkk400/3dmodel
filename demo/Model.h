#pragma once
#include "headers.h"
#include "Mesh.h"
class Model
{
public:

	explicit Model(QObject *parent = nullptr);
	void Load(std::string filepath);
	void processNode(aiNode* node, const aiScene *scene);		//�ݹ鴦��ÿ���ڵ㣬����״�ṹ
	void processMesh(aiMesh* aimesh, const aiScene *scene);

	std::vector<Mesh> meshes;
	std::map<std::string, GLuint> textureMap;
	std::string rootPath;
	QOpenGLTexture* texture;
	std::string fpath;

};

