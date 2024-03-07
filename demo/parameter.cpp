#include "parameter.h"

std::string parameter::path = "";

bool parameter::isdrawAxis = true;
bool parameter::isdrawSky = true;
bool parameter::isdrawGround = true;
bool parameter::isdrawcompositeCube = false;
bool parameter::isdrawStatuspng = true;
bool parameter::isdrawLight = true;


QVector3D parameter::lightPosition = QVector3D(5.0f, 5.0f, 5.0f);
float parameter::ambient_strength = 0.5;
QVector3D parameter::lightColor = QVector3D(1.0, 1.0, 1.0);

float parameter::fov = 60.0f;			
float parameter::sensitivity = 1.5;		
float parameter::cameraSpeed = 0.1f;
float parameter::r = 10.0f;
float parameter::near_ = 0.01f;
float parameter::far_ = 30.0f;
QVector3D parameter::viewDirection = QVector3D(0.0f, 0.0f, 0.0f);
QVector3D parameter::cameraDirection = QVector3D(0.0f, 0.0f, parameter::r);
QVector3D parameter::cameraUp = QVector3D(0.0f, 1.0f, 0.0f);     

QMatrix4x4 parameter::view = QMatrix4x4();
QMatrix4x4 parameter::projection = QMatrix4x4();


