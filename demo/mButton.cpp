#include "mButton.h"

mButton::mButton(QString image1, QString image2, int index)
{
	this->index = index;

	setCursor(QCursor(Qt::PointingHandCursor));

	QPixmap pixmap1(image1);
	fitpixmap1 = pixmap1.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap pixmap2(image2);
	fitpixmap2 = pixmap2.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	isselect = false;

	setIconSize(QSize(40, 40));
	setFlat(true);
	setStyleSheet("border: 0px");
	setIcon1();
}


mButton::~mButton()
{
}

void mButton::setIcon1()
{
	setIcon(QIcon(fitpixmap1));
	isselect = false;
}

void mButton::setIcon2()
{
	setIcon(QIcon(fitpixmap2));
	isselect = true;
}

void mButton::mousePressEvent(QMouseEvent *)
{
	if (!isselect) {
		setIcon2();
		isselect = true;
		emit isclick(index);
	}
}
