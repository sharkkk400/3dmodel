#pragma once
#include <qvector3d.h>
#include <qmatrix4x4.h>
#include <iostream>
#include <string>
class parameter
{
public:

	static std::string path;					//����·��

	static bool isdrawAxis;						//�Ƿ����������
	static bool isdrawSky;						//�Ƿ������պ�
	static bool isdrawGround;					//�Ƿ���Ƶذ�
	static bool isdrawcompositeCube;			//�Ƿ���Ƹ���ģ����ӿ�
	static bool isdrawStatuspng;				//�Ƿ����״̬���
	static bool isdrawLight;					//�Ƿ��Դ��


	static QVector3D lightPosition;				//��Դλ��
	static float ambient_strength;				//������ǿ
	static QVector3D lightColor;				//��Դ��ɫ

	static QVector3D viewDirection;				//�ӵ�λ��
	static QVector3D cameraDirection;			//�������
	static QVector3D cameraUp;					//���������

	static QMatrix4x4 view;						//��ͼ����
	static QMatrix4x4 projection;				//ͶӰ����

	static float fov;							//�����Ұ��Χ
	static float sensitivity;					//���������
	static float cameraSpeed;					//����ƶ��ٶ�
	static float r;								//������Ե�ľ���
	static float near_;							//�����Ⱦ����
	static float far_;							//��Զ��Ⱦ����
	
};

