#include "moveAxis.h"

moveAxis::moveAxis()
{
	XYZ = sphere(0.2);
	components.push_back(&XYZ);

	Xcylinder = cylinder(1.5, 0.05);
	Xcylinder.color = QVector3D(1, 0, 0);
	Xcylinder.meshModel.translate(1, 0, 0);
	Xcylinder.meshModel.rotate(270, 0, 0, 1);
	components.push_back(&Xcylinder);

	Xcone = Cone(0.1, 0.5);
	Xcone.color = QVector3D(1, 0, 0);
	Xcone.meshModel.translate(1.75, 0, 0);
	Xcone.meshModel.rotate(270, 0, 0, 1);
	components.push_back(&Xcone);

	Ycylinder = cylinder(1.5,0.05);
	Ycylinder.color = QVector3D(0, 1, 0);
	Ycylinder.meshModel.translate(0, 1, 0);
	components.push_back(&Ycylinder);

	Ycone = Cone(0.1, 0.5);
	Ycone.color = QVector3D(0, 1, 0);
	Ycone.meshModel.translate(0, 1.75, 0);
	components.push_back(&Ycone);

	Zcylinder = cylinder(1.5, 0.05);
	Zcylinder.color = QVector3D(0, 0, 1);
	Zcylinder.meshModel.translate(0, 0, 1);
	Zcylinder.meshModel.rotate(90, 1, 0, 0);
	components.push_back(&Zcylinder);

	Zcone = Cone(0.1, 0.5);
	Zcone.color = QVector3D(0, 0, 1);
	Zcone.meshModel.translate(0, 0, 1.75);
	Zcone.meshModel.rotate(90, 1, 0, 0);
	components.push_back(&Zcone);

	XOY = square(0.25,0.25);
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

moveAxis::~moveAxis()
{
}

bool moveAxis::checkin(QVector2D viewXY)
{
	bool flag = false;
	for (auto mesh : components) {
		if (mesh->checkSelect(viewXY) && flag == false) {
			flag = true;
			mesh->alpha = 1.0;
			if (mesh == &XYZ) {
				item = 1;
			}
			if (mesh == &Xcylinder|| mesh == &Xcone) {
				item = 2;
			}
			if (mesh == &Ycylinder || mesh == &Ycone) {
				item = 3;
			}
			if (mesh == &Zcylinder || mesh == &Zcone) {
				item = 4;
			}
			if (mesh == &XOZ) {
				item = 5;
			}
			if (mesh == &XOY) {
				item = 6;
			}
			if (mesh == &YOZ) {
				item = 7;
			}
		}
		else {
			mesh->alpha = 0.6;
		}
	}
	unifycylinderandcone();
	return flag;
}

void moveAxis::unifycylinderandcone()
{
	if (Xcylinder.alpha == 1.0 || Xcone.alpha == 1.0) {
		Xcylinder.alpha = 1.0;
		Xcone.alpha = 1.0;
	}
	if (Ycylinder.alpha == 1.0 || Ycone.alpha == 1.0) {
		Ycylinder.alpha = 1.0;
		Ycone.alpha = 1.0;
	}
	if (Zcylinder.alpha == 1.0 || Zcone.alpha == 1.0) {
		Zcylinder.alpha = 1.0;
		Zcone.alpha = 1.0;
	}

}
