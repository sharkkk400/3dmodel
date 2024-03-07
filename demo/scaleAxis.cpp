#include "scaleAxis.h"

scaleAxis::scaleAxis()
{
	XYZ = sphere(0.2);
	components.push_back(&XYZ);

	Xcylinder = cylinder(1.5, 0.05);
	Xcylinder.color = QVector3D(1, 0, 0);
	Xcylinder.meshModel.translate(1, 0, 0);
	Xcylinder.meshModel.rotate(270, 0, 0, 1);
	components.push_back(&Xcylinder);

	Xcube = cube(0.25, 0.25, 0.25);
	Xcube.color = QVector3D(1, 0, 0);
	Xcube.meshModel.translate(1.75, 0, 0);
	Xcube.meshModel.rotate(270, 0, 0, 1);
	components.push_back(&Xcube);

	Ycylinder = cylinder(1.5, 0.05);
	Ycylinder.color = QVector3D(0, 1, 0);
	Ycylinder.meshModel.translate(0, 1, 0);
	components.push_back(&Ycylinder);

	Ycube = cube(0.25, 0.25, 0.25);
	Ycube.color = QVector3D(0, 1, 0);
	Ycube.meshModel.translate(0, 1.75, 0);
	components.push_back(&Ycube);

	Zcylinder = cylinder(1.5, 0.05);
	Zcylinder.color = QVector3D(0, 0, 1);
	Zcylinder.meshModel.translate(0, 0, 1);
	Zcylinder.meshModel.rotate(90, 1, 0, 0);
	components.push_back(&Zcylinder);

	Zcube = cube(0.25, 0.25, 0.25);
	Zcube.color = QVector3D(0, 0, 1);
	Zcube.meshModel.translate(0, 0, 1.75);
	Zcube.meshModel.rotate(90, 1, 0, 0);
	components.push_back(&Zcube);

	XOY = square(0.25, 0.25);
	XOY.color = QVector3D(0, 0, 1);
	XOY.meshModel.translate(0.75, 0.75, 0);
	XOY.meshModel.rotate(90, 1, 0, 0);
	components.push_back(&XOY);

	XOZ = square(0.25, 0.25);
	XOZ.color = QVector3D(0, 1, 0);
	XOZ.meshModel.translate(0.75, 0, 0.75);
	components.push_back(&XOZ);

	YOZ = square(0.25, 0.25);
	YOZ.color = QVector3D(1, 0, 0);
	YOZ.meshModel.translate(0, 0.75, 0.75);
	YOZ.meshModel.rotate(90, 0, 0, 1);
	components.push_back(&YOZ);

	unititem[1] = 2;
	unititem[2] = 1;
	unititem[3] = 4;
	unititem[4] = 3;
	unititem[5] = 6;
	unititem[6] = 5;
}

scaleAxis::~scaleAxis()
{
}

