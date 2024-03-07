#pragma once
#include "headers.h"
#include "Mesh.h"
class Model
{
public:

	explicit Model(QObject *parent = nullptr);
	void Load(std::string filepath);
	void processNode(aiNode* node, const aiScene *scene);		//递归处理每个节点，成树状结构
	void processMesh(aiMesh* aimesh, const aiScene *scene);

	std::vector<Mesh> meshes;
	std::map<std::string, GLuint> textureMap;
	std::string rootPath;
	QOpenGLTexture* texture;
	std::string fpath;

};

