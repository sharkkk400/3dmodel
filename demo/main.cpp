#pragma once
#include "headers.h"
#include "mMainWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	mMainWindow w;
	w.show();
	return app.exec();
}