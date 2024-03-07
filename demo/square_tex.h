#pragma once
#include "square.h"
class square_tex : public square
{
public:
	square_tex(QString pngPath = "",float x = 1, float z = 1);
	~square_tex();


	QString pngPath;
	std::vector<QVector2D> texcoord;
	QOpenGLTexture *Texture;
	void bind();
	void draw(QMatrix4x4 base = QMatrix4x4());
};

