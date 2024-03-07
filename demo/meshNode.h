#pragma once
#include "headers.h"
#include "Mesh.h"

class meshNode
{
public:

	meshNode(Mesh* mesh = nullptr, meshNode* parent = nullptr);
	~meshNode();

	QString name;						//���ƣ���mesh�е�name��ͬ����������
	Mesh* mesh;							//meshʵ��
	bool isDraw;						//�Ƿ����mesh

	QMatrix4x4 baseModel;				//��̬�任����
	QMatrix4x4 meshModel;				//�ֲ��任����

	meshNode* parent;					//����mesh
	std::set<meshNode*> children;		//�Ӽ�mesh

	void binddata();
	void drawMesh();
	void updateModel();
	void correctModel();
	void destory();

};

