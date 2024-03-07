#pragma once
#include "headers.h"
#include "Mesh.h"

class meshNode
{
public:

	meshNode(Mesh* mesh = nullptr, meshNode* parent = nullptr);
	~meshNode();

	QString name;						//名称，和mesh中的name相同，用于索引
	Mesh* mesh;							//mesh实例
	bool isDraw;						//是否绘制mesh

	QMatrix4x4 baseModel;				//基态变换矩阵
	QMatrix4x4 meshModel;				//局部变换矩阵

	meshNode* parent;					//父级mesh
	std::set<meshNode*> children;		//子级mesh

	void binddata();
	void drawMesh();
	void updateModel();
	void correctModel();
	void destory();

};

