#include "rotateAxis.h"

rotateAxis::rotateAxis()
{
	X = arc();
	X.color = QVector3D(1, 0, 0);
	X.meshModel.rotate(90, 0, 0, 1);
	X.linewidth = 5.0f;
	components.push_back(&X);

	Y = arc();
	Y.color = QVector3D(0, 1, 0);
	Y.linewidth = 5.0f;
	components.push_back(&Y);

	Z = arc();
	Z.color = QVector3D(0, 0, 1);
	Z.meshModel.rotate(90, 1, 0, 0);
	Z.linewidth = 5.0f;
	components.push_back(&Z);

}

rotateAxis::~rotateAxis()
{
}