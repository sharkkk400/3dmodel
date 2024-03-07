#include "workclass.h"
workclass::workclass(QObject* parent) {

}
void workclass::loadmesh(QStringList pathlist)
{
	for (QString path : pathlist) {
		Model m;
		m.Load(std::string(path.toLocal8Bit()));
		emit passmesh(m.meshes);
	}
	emit successload();

}
