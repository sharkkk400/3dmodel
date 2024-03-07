#pragma once
#include "headers.h"
class mButton : public QPushButton
{
	Q_OBJECT
public:
	mButton(QString image1, QString image2, int index);
	~mButton();

	QPixmap fitpixmap1;
	QPixmap fitpixmap2;
	bool isselect;
	int index;

	void setIcon1();
	void setIcon2();

public slots:
	void mousePressEvent(QMouseEvent*) override;

signals:
	void isclick(int index);
};

