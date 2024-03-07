#pragma once
#include <qobject.h>
#include "headers.h"
#include "Model.h"
class workclass :
	public QObject
{
	Q_OBJECT
public:
	workclass(QObject* parent = nullptr);

public Q_SLOTS:
	void loadmesh(QStringList pathlist);

signals:
	void successload();
	void passmesh(std::vector<Mesh> meshes);
};

