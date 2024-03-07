#pragma once
#include <qvector3d.h>
#include <qmatrix4x4.h>
#include <iostream>
#include <string>
class parameter
{
public:

	static std::string path;					//工作路径

	static bool isdrawAxis;						//是否绘制坐标轴
	static bool isdrawSky;						//是否绘制天空盒
	static bool isdrawGround;					//是否绘制地板
	static bool isdrawcompositeCube;			//是否绘制复合模型外接框
	static bool isdrawStatuspng;				//是否绘制状态面板
	static bool isdrawLight;					//是否光源点


	static QVector3D lightPosition;				//光源位置
	static float ambient_strength;				//环境光强
	static QVector3D lightColor;				//光源颜色

	static QVector3D viewDirection;				//视点位置
	static QVector3D cameraDirection;			//相机方向
	static QVector3D cameraUp;					//相机上向量

	static QMatrix4x4 view;						//视图矩阵
	static QMatrix4x4 projection;				//投影矩阵

	static float fov;							//相机视野范围
	static float sensitivity;					//鼠标灵敏度
	static float cameraSpeed;					//相机移动速度
	static float r;								//相机与试点的距离
	static float near_;							//最近渲染距离
	static float far_;							//最远渲染距离
	
};

