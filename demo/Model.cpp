#include "Model.h"

Model::Model(QObject *parent)
{
}

void Model::Load(std::string filepath)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);
	// 异常处理
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		exit(-1);
	}
	// 模型文件相对路径
	rootPath = filepath.substr(0, filepath.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (int i = 0; i < node->mNumMeshes; i++) {
		//将数据传入自定义mesh中
		processMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	
	//递归处理
	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
	
}

void Model::processMesh(aiMesh* aimesh, const aiScene *scene)
{
	Mesh mesh;
	mesh.rootpath = QString::fromStdString(fpath);
	// 位置、法线、纹理
	for (int j = 0; j < aimesh->mNumVertices; j++)
	{

		// 顶点
		QVector3D vvv;
		vvv.setX(aimesh->mVertices[j].x);
		vvv.setY(aimesh->mVertices[j].y);
		vvv.setZ(aimesh->mVertices[j].z);
		mesh.vertexPosition.push_back(vvv);
		// 法线
		QVector3D nnn;
		nnn.setX(aimesh->mNormals[j].x);
		nnn.setY(aimesh->mNormals[j].y);
		nnn.setZ(aimesh->mNormals[j].z);
		mesh.vertexNormal.push_back(nnn);
		// 纹理坐标: 如果存在则加入。assimp 默认可以有多个纹理坐标 我们取第一个（0）即可
		QVector2D tt(0, 0);
		tt.setX(aimesh->mTextureCoords[0][j].x);
		tt.setY(aimesh->mTextureCoords[0][j].y);
		mesh.vertexTexcoord.push_back(tt);
	}

	mesh.boxCoord.resize(6);
	mesh.boxCoord[0] = mesh.vertexPosition[0][0];		//最大x坐标
	mesh.boxCoord[1] = mesh.vertexPosition[0][1];		//最大y坐标
	mesh.boxCoord[2] = mesh.vertexPosition[0][2];		//最大z坐标
	mesh.boxCoord[3] = mesh.vertexPosition[1][0];		//最小x坐标
	mesh.boxCoord[4] = mesh.vertexPosition[1][1];		//最小y坐标
	mesh.boxCoord[5] = mesh.vertexPosition[1][2];		//最小z坐标

	//构造外包立方体
	for (int i = 0; i < mesh.vertexPosition.size(); i++) {
		if (mesh.vertexPosition[i][0] > mesh.boxCoord[0]) {
			mesh.boxCoord[0] = mesh.vertexPosition[i][0];
		}
		if (mesh.vertexPosition[i][1] > mesh.boxCoord[1]) {
			mesh.boxCoord[1] = mesh.vertexPosition[i][1];
		}
		if (mesh.vertexPosition[i][2] > mesh.boxCoord[2]) {
			mesh.boxCoord[2] = mesh.vertexPosition[i][2];
		}
		if (mesh.vertexPosition[i][0] < mesh.boxCoord[3]) {
			mesh.boxCoord[3] = mesh.vertexPosition[i][0];
		}
		if (mesh.vertexPosition[i][1] < mesh.boxCoord[4]) {
			mesh.boxCoord[4] = mesh.vertexPosition[i][1];
		}
		if (mesh.vertexPosition[i][2] < mesh.boxCoord[5]) {
			mesh.boxCoord[5] = mesh.vertexPosition[i][2];
		}
	}
	{
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[1], mesh.boxCoord[2]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[4], mesh.boxCoord[2]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[4], mesh.boxCoord[2]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[4], mesh.boxCoord[5]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[4], mesh.boxCoord[5]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[1], mesh.boxCoord[5]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[1], mesh.boxCoord[5]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[1], mesh.boxCoord[2]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[1], mesh.boxCoord[2]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[4], mesh.boxCoord[2]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[4], mesh.boxCoord[2]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[4], mesh.boxCoord[5]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[4], mesh.boxCoord[5]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[1], mesh.boxCoord[5]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[1], mesh.boxCoord[5]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[1], mesh.boxCoord[2]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[1], mesh.boxCoord[2]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[1], mesh.boxCoord[2]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[4], mesh.boxCoord[2]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[4], mesh.boxCoord[2]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[4], mesh.boxCoord[5]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[4], mesh.boxCoord[5]));

		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[0], mesh.boxCoord[1], mesh.boxCoord[5]));
		mesh.boxVertex.push_back(QVector3D(mesh.boxCoord[3], mesh.boxCoord[1], mesh.boxCoord[5]));
	}

	mesh.boxCenter.setX((mesh.boxCoord[0] + mesh.boxCoord[3]) / 2);
	mesh.boxCenter.setY((mesh.boxCoord[1] + mesh.boxCoord[4]) / 2);
	mesh.boxCenter.setZ((mesh.boxCoord[2] + mesh.boxCoord[5]) / 2);

	//构造局部坐标系
	float XL = mesh.boxCoord[0] - mesh.boxCoord[3];
	float YL = mesh.boxCoord[1] - mesh.boxCoord[4];
	float ZL = mesh.boxCoord[2] - mesh.boxCoord[5];
	mesh.lenth = (XL > YL ? XL : YL);
	mesh.lenth = (mesh.lenth > ZL ? mesh.lenth : ZL);
	mesh.lenth *= 0.6;

	if (aimesh->mMaterialIndex >= 0)
	{
		// 获取当前 aimesh 的材质对象
		aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];
		
		float shiness;
		material->Get(AI_MATKEY_SHININESS, shiness);
		if (shiness) {
			mesh.flag = true;
			mesh.Ns = shiness;
			aiColor3D color;
			material->Get(AI_MATKEY_COLOR_AMBIENT, color);
			mesh.Ka = QVector3D(color.r, color.g, color.b);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			mesh.Kd = QVector3D(color.r, color.g, color.b);
			material->Get(AI_MATKEY_COLOR_SPECULAR, color);
			mesh.Ks = QVector3D(color.r, color.g, color.b);
			
		}

		/*
		// 获取 diffuse 贴图文件路径名称 我们只取1张贴图 故填 0 即可
		aiString aistr;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &aistr);
		std::string texpath = aistr.C_Str();
		texpath = rootPath + '/' + texpath;   // 取相对路径

		// 如果没生成过纹理，那么生成它
		if (textureMap.find(texpath) == textureMap.end())
		{
			// 生成纹理
			GLuint tex;
			texture = new QOpenGLTexture(QImage(QString::fromStdString(texpath)).mirrored());
			texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
			texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
			texture->setMinificationFilter(QOpenGLTexture::Linear);
			texture->setMagnificationFilter(QOpenGLTexture::Linear);
			textureMap[texpath] = texture->textureId();
		}

		// 传递纹理
		mesh.diffuseTexture = textureMap[texpath];*/
	}

	// 面片索引
	for (GLuint j = 0; j < aimesh->mNumFaces; j++) {
		aiFace face = aimesh->mFaces[j];
		for (GLuint k = 0; k < face.mNumIndices; k++) {
			mesh.index.push_back(face.mIndices[k]);
		}
	}

	// 数据绑定
	//mesh.bindData();

	meshes.push_back(mesh);
}
