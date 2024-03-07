#include "meshNode.h"

meshNode::meshNode(Mesh * mesh, meshNode * parent)
{
	this->mesh = mesh;
	this->parent = parent;
	baseModel.setToIdentity();
	meshModel.setToIdentity();
	if (parent) {
		parent->children.insert(this);
	}
	this->name = mesh ? mesh->meshName : "";
	children.clear();
	isDraw = true;
	
}

meshNode::~meshNode()
{
	for (auto node : children) {
		delete node;
	}
	delete mesh;
}

void meshNode::binddata()
{
	if( mesh && !mesh->isbind ){
		mesh->bindData();
	}

	for (auto node : children) {
		node->binddata();
	}
}

void meshNode::drawMesh()
{
	if (mesh&&isDraw) {
		mesh->draw();
	}
	
	for (auto node : children) {
		node->drawMesh();
	}
}

void meshNode::updateModel()
{	
	correctModel();
	if (mesh) {
		mesh->updateModel();
	}
	
}

void meshNode::correctModel()
{
	if (mesh) {
		if (parent->mesh) {
			//将子节点的坐标原点移动为父节点的几何中心
			QMatrix4x4 correct;
			correct.translate(parent->mesh->boxCenter);
			mesh->correctModel(baseModel*correct);
		}
		else {
			mesh->correctModel(baseModel);
		}
	}

	for (auto child : children) {
		child->baseModel = baseModel;
		if (mesh) {
			child->baseModel *= mesh->meshModel;
		}
		child->correctModel();
	}
}

void meshNode::destory()
{
	parent->children.erase(this);
}
