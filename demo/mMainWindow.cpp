#include "mMainWindow.h"

mMainWindow::mMainWindow(QMainWindow* parent)
{
	this->resize(1524, 868);//窗口大小
	this->setWindowIcon(QIcon(QString::fromStdString(parameter::path) + "data/1.ico"));


	//获取工作目录
	parameter::path = std::experimental::filesystem::current_path().string();
	for (int i = 0; i < parameter::path.size(); i++) {
		if (parameter::path[i] == '\\') {
			parameter::path[i] = '/';
		}
	}
	parameter::path = parameter::path.substr(0, parameter::path.find_last_of('/')+1);

	//初始化窗口
	initMenuBar();
	initModelShow();
	initModelTree();
	initModelAttribute();
	initSceneParaWidget();
	initCombinationAttribute();

	QStatusBar *statusBr = statusBar();
	statusBr->addWidget(qopengl->wps);

	//加载模型时，动态更新模型树
	connect(qopengl, SIGNAL(updateTree(QString)), this, SLOT(appendTree(QString)));

	//点击模型树，显示外接立方体和对应mesh属性
	connect(ModelTree, &QTreeWidget::itemPressed, this, [&](QTreeWidgetItem *item, int column) {
		updatemeshNode();
		selectedItem = item;
		QString name = item->text(0);
		for (auto it = qopengl->nodeMap.begin(); it != qopengl->nodeMap.end(); it++) {
			if (it->first != name) {
				it->second->mesh->isSelect = false;
			}
			else {
				it->second->mesh->isSelect = true;
				getModelAttribute(item);
				qopengl->initcompositeCube(item2node(item));
			}
		}
		qopengl->update();
		});

}

mMainWindow::~mMainWindow()
{
}

void mMainWindow::keyReleaseEvent(QKeyEvent * event)
{
	if (!filter) {
		qopengl->keyReleaseEvent(event);
	}
}

void mMainWindow::initMenuBar()
{
	//创建菜单栏，只能有一个
	m_menuBar = menuBar();

	//将菜单栏放入窗口中
	this->setMenuBar(m_menuBar);

	//创建文件菜单
	QMenu *fileMenu = m_menuBar->addMenu(u8"文件");
	QMenu *editMenu = m_menuBar->addMenu(u8"变换");

	//添加菜单里的选项
	QAction *newAction = fileMenu->addAction(u8"导入模型");
	connect(newAction, SIGNAL(triggered()), this, SLOT(LoadModel()));

	/*
	loader = new workclass;
	loader->moveToThread(&loadThread);

	connect(this, &mMainWindow:: beginLoad, loader, &workclass::loadmesh);
	connect(&loadThread, &QThread::finished, loader, &QObject::deleteLater);
	connect(loader, &workclass::passmesh, this, &mMainWindow::passmesh);

	loadThread.start();*/

	//添加分割线
	fileMenu->addSeparator();
	QAction *moreAction = fileMenu->addAction(u8"···");

}

void mMainWindow::initModelTree()
{
	m_DockWidget = new QDockWidget(this);
	m_DockWidget->setWindowTitle(u8"模型列表");
	m_DockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	m_DockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	this->addDockWidget(Qt::LeftDockWidgetArea, m_DockWidget);

	ModelTree = new QTreeWidget();
	ModelTree->setHeaderHidden(true);
	ModelTree->setContextMenuPolicy(Qt::CustomContextMenu);

	itemMenu = new QMenu(ModelTree);
	QAction *deleteMesh = itemMenu->addAction(u8"删除");
	connect(deleteMesh, SIGNAL(triggered()), this, SLOT(DeleteModel()));
	blankMenu = new QMenu(ModelTree);
	QAction *addMesh = blankMenu->addAction(u8"添加");
	QAction *renew = blankMenu->addAction(u8"更新");
	connect(addMesh, SIGNAL(triggered()), this, SLOT(LoadModel()));
	connect(renew, &QAction::triggered, this, [&]() {
		qopengl->update();
	});
	
	connect(ModelTree, &QTreeWidget::itemClicked, this, &mMainWindow::slotClicked);
	connect(ModelTree, &QTreeWidget::customContextMenuRequested, this, [&](const QPoint &pos) {
		QTreeWidgetItem* curItem = ModelTree->itemAt(pos);
		if (curItem) {
			itemMenu->exec(QCursor::pos());
		}
		else {
			if (selectedItem) {
				ModelTree->setItemSelected(selectedItem, false);
				qopengl->nodeMap[selectedItem->text(0)]->mesh->isSelect = false;
			}
			
			selectedItem = nullptr;
			qopengl->initcompositeCube(item2node(selectedItem));
			qopengl->update();
			getModelAttribute(selectedItem);
			blankMenu->exec(QCursor::pos());
		}
	});

	ModelTree->setDragEnabled(true);
	ModelTree->setDragDropMode(QAbstractItemView::InternalMove);
	ModelTree->setDefaultDropAction(Qt::MoveAction);

	m_DockWidget->setWidget(ModelTree);

	ModelTree->viewport()->installEventFilter(this);

}

void mMainWindow::setCheckStatus(QTreeWidgetItem * item, Qt::CheckState s)
{
	for (int i = 0; i < item->childCount(); i++)
	{
		item->child(i)->setCheckState(0, s);
		if (s == Qt::Checked) {
			item->child(i)->setCheckState(0, Qt::Checked);
			qopengl->nodeMap[item->child(i)->text(0)]->isDraw = true;
		}
		else {
			item->child(i)->setCheckState(0, Qt::Unchecked);
			qopengl->nodeMap[item->child(i)->text(0)]->isDraw = false;
		}
		
		setCheckStatus(item->child(i), s);
	}
}

void mMainWindow::slotClicked(QTreeWidgetItem *item, int column)
{

	if (item->checkState(0) == Qt::Checked) {
		qopengl->nodeMap[item->text(0)]->isDraw = true;
	}
	else {
		qopengl->nodeMap[item->text(0)]->isDraw = false;
	}

	setCheckStatus(item, item->checkState(0));

	qopengl->update();
}

void mMainWindow::updateModel()
{
	if (!selectedItem) { return; }

	meshNode* node = item2node(selectedItem);

	node->mesh->scaleX = ScaleX->text(1).toDouble();
	node->mesh->scaleY = ScaleY->text(1).toDouble();
	node->mesh->scaleZ = ScaleZ->text(1).toDouble();
	node->mesh->rotateX = RoatationX->text(1).toFloat();
	node->mesh->rotateY = RoatationY->text(1).toFloat();
	node->mesh->rotateZ = RoatationZ->text(1).toFloat();
	node->mesh->target_wpsX = PositionX->text(1).toDouble();
	node->mesh->target_wpsY = PositionY->text(1).toDouble();
	node->mesh->target_wpsZ = PositionZ->text(1).toDouble();
	node->updateModel();
	//node->correctModel();//更新变换矩阵后一定要进行父子矩阵矫正
	node->mesh->objectColor.setX(colorpicker->getColor().redF());
	node->mesh->objectColor.setY(colorpicker->getColor().greenF());
	node->mesh->objectColor.setZ(colorpicker->getColor().blueF());

	if (selectedItem->childCount()) {
		qopengl->initcompositeCube(node);
	}

	qopengl->update();
}

void mMainWindow::updateCombineModel()
{/*
	for (int i = 0; i < meshlist.size();i++) {
		//缩放
		
		meshlist[i]->scaleX *= combinationScaleX->text(1).toDouble();
		meshlist[i]->scaleY *= combinationScaleY->text(1).toDouble();
		meshlist[i]->scaleZ *= combinationScaleZ->text(1).toDouble();
		QVector3D SV = QVector3D(meshlist[i]->target_wps.x() - qopengl->compositeCube->center.x(),
			meshlist[i]->target_wps.y() - qopengl->compositeCube->center.y(),
			meshlist[i]->target_wps.z() - qopengl->compositeCube->center.z());
		SV.setX(SV.x() * combinationScaleX->text(1).toDouble());
		SV.setY(SV.y() * combinationScaleY->text(1).toDouble());
		SV.setZ(SV.z() * combinationScaleZ->text(1).toDouble());
		meshlist[i]->target_wpsX = SV.x() + qopengl->compositeCube->center.x();
		meshlist[i]->target_wpsY = SV.y() + qopengl->compositeCube->center.y();
		meshlist[i]->target_wpsZ = SV.z() + qopengl->compositeCube->center.z();

		qopengl->initcompositeCube(meshlist);

		//旋转
		meshlist[i]->rotateX += combinationRoatationX->text(1).toDouble();
		meshlist[i]->rotateY += combinationRoatationY->text(1).toDouble();
		meshlist[i]->rotateZ += combinationRoatationZ->text(1).toDouble();
		QVector3D v = meshlist[i]->target_wps - qopengl->compositeCube->center;
		QMatrix4x4 m;
		m.rotate(combinationRoatationX->text(1).toDouble(), 1, 0, 0);
		m.rotate(combinationRoatationY->text(1).toDouble(), 0, 1, 0);
		m.rotate(combinationRoatationZ->text(1).toDouble(), 0, 0, 1);
		QVector3D vv = m * v;
		meshlist[i]->target_wpsX = vv.x() + qopengl->compositeCube->center.x();
		meshlist[i]->target_wpsY = vv.y() + qopengl->compositeCube->center.y();
		meshlist[i]->target_wpsZ = vv.z() + qopengl->compositeCube->center.z();

		qopengl->initcompositeCube(meshlist);


		QVector3D mTranslate = QVector3D(combinationPositionX->text(1).toDouble() - qopengl->compositeCube->center.x(),
			combinationPositionY->text(1).toDouble() - qopengl->compositeCube->center.y(),
			combinationPositionZ->text(1).toDouble() - qopengl->compositeCube->center.z());

		meshlist[i]->target_wpsX += mTranslate.x();
		meshlist[i]->target_wpsY += mTranslate.y();
		meshlist[i]->target_wpsZ += mTranslate.z();


		meshlist[i]->updateModel();
	}
	qopengl->initcompositeCube(meshlist);
	qopengl->update();*/
}

void mMainWindow::updatemeshNode()
{ 
	qopengl->Scene->children.clear();
	for (int i = 0; i < ModelTree->topLevelItemCount(); i++) {
		meshNode* node = item2node(ModelTree->topLevelItem(i));
		node->parent = qopengl->Scene;
		qopengl->Scene->children.insert(node);
		node->children.clear();
		for (int j = 0; j < ModelTree->topLevelItem(i)->childCount(); j++) {
			node->children.insert(item2node(ModelTree->topLevelItem(i)->child(j)));
			updatesubnode(ModelTree->topLevelItem(i)->child(j), node);
		}
	}
	qopengl->Scene->correctModel();
}

void mMainWindow::updatesubnode(QTreeWidgetItem * item, meshNode * parent)
{
	meshNode* node = item2node(item);
	node->parent = parent;
	node->children.clear();
	for (int i = 0; i < item->childCount(); i++) {
		node->children.insert(item2node(item->child(i)));
		updatesubnode(item->child(i), node);
	}
}

void mMainWindow::getModelAttribute(QTreeWidgetItem * item)
{
	RootPath->setText(1, item ? qopengl->nodeMap[item->text(0)]->mesh->rootpath : "");
	ModelName->setText(1, item ? qopengl->nodeMap[item->text(0)]->mesh->meshName : "");
	ScaleX->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->scaleX) : "");
	ScaleY->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->scaleY) : "");
	ScaleZ->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->scaleZ) : "");
	PositionX->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->target_wps.x()) : "");
	PositionY->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->target_wps.y()) : "");
	PositionZ->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->target_wps.z()) : "");
	RoatationX->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->rotateX) : "");
	RoatationY->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->rotateY) : "");
	RoatationZ->setText(1, item ? QString::number(qopengl->nodeMap[item->text(0)]->mesh->rotateZ) : "");
	QColor c;
	c.setRgbF(item ? qopengl->nodeMap[item->text(0)]->mesh->objectColor.x() : 0.0f,
		item ? qopengl->nodeMap[item->text(0)]->mesh->objectColor.y() : 0.0f,
		item ? qopengl->nodeMap[item->text(0)]->mesh->objectColor.z() : 0.0f);
	colorpicker->setColor(c);
}

void mMainWindow::getsubmodel(QTreeWidgetItem * item, QTreeWidgetItem * parent)
{
	for (int i = 0; i < item->childCount(); i++) {
		QTreeWidgetItem* submodel = new QTreeWidgetItem(QStringList() << item->child(i)->text(0));
		submodel->setCheckState(0, Qt::Checked);
		parent->addChild(submodel);
		itemmapmodel[item->child(i)->text(0)] = submodel;
		getsubmodel(item->child(i),submodel);
	}
}

void mMainWindow::deletesubmodel(QTreeWidgetItem * item)
{
	for (int i = 0; i < item->childCount(); i++) {
		deletesubmodel(item->child(i));
	}
	delete item;
}

void mMainWindow::getselecetedmesh(QTreeWidgetItem * item)
{
	meshlist.append(qopengl->nodeMap[item->text(0)]->mesh);
	for (int i = 0; i < item->childCount(); i++) {
		getselecetedmesh(item->child(i));
	}
}

void mMainWindow::initSceneParaWidget()
{
	m_SceneParaWidget = new QDockWidget(this);
	m_SceneParaWidget->setWindowTitle(u8"场景参数");
	m_SceneParaWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	m_SceneParaWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	this->addDockWidget(Qt::LeftDockWidgetArea, m_SceneParaWidget);

	m_ScenePara = new QTreeWidget();
	m_ScenePara->setHeaderLabels(QStringList() << u8"参数" << u8"值");

	m_SceneParaWidget->setWidget(m_ScenePara);

	skyPara = new QTreeWidgetItem(QStringList() << u8"天空盒");
	m_ScenePara->addTopLevelItem(skyPara);
	drawsky = new QCheckBox();
	drawsky->setChecked(true);
	m_ScenePara->setItemWidget(skyPara, 1, drawsky);
	connect(drawsky, &QCheckBox::stateChanged, [&](int state) {
		if (state == Qt::Checked) {
			parameter::isdrawSky = true;
		}
		else {
			parameter::isdrawSky = false;
		}
		qopengl->update();
		});

	groundPara = new QTreeWidgetItem(QStringList() << u8"地板");
	m_ScenePara->addTopLevelItem(groundPara);
	drawground = new QCheckBox();
	drawground->setChecked(true);
	m_ScenePara->setItemWidget(groundPara, 1, drawground);
	connect(drawground, &QCheckBox::stateChanged, [&](int state) {
		if (state == Qt::Checked) {
			parameter::isdrawGround = true;
		}
		else {
			parameter::isdrawGround = false;
		}
		qopengl->update();
		});
	
	AixsPara = new QTreeWidgetItem(QStringList() << u8"坐标系");
	m_ScenePara->addTopLevelItem(AixsPara);
	drawAixs = new QCheckBox();
	drawAixs->setChecked(true);
	m_ScenePara->setItemWidget(AixsPara, 1, drawAixs);
	connect(drawAixs, &QCheckBox::stateChanged, [&](int state) {
		if (state == Qt::Checked) {
			parameter::isdrawAxis = true;
		}
		else {
			parameter::isdrawAxis = false;
		}
		qopengl->update();
		});

	StatusPara = new QTreeWidgetItem(QStringList() << u8"状态栏");
	m_ScenePara->addTopLevelItem(StatusPara);
	drawStatus = new QCheckBox();
	drawStatus->setChecked(true);
	m_ScenePara->setItemWidget(StatusPara, 1, drawStatus);
	connect(drawStatus, &QCheckBox::stateChanged,[&](int state) {
		if (state == Qt::Checked) {
			parameter::isdrawStatuspng = true;
		}
		else {
			parameter::isdrawStatuspng = false;
		}
		qopengl->update();
		});

	LightPara = new QTreeWidgetItem(QStringList() << u8"光源");
	m_ScenePara->addTopLevelItem(LightPara);
	drawLight = new QCheckBox();
	drawLight->setChecked(true);
	m_ScenePara->setItemWidget(LightPara, 1, drawLight);
	connect(drawLight, &QCheckBox::stateChanged, [&](int state) {
		if (state == Qt::Checked) {
			parameter::isdrawLight = true;
		}
		else {
			parameter::isdrawLight = false;
		}
		qopengl->update();
		});

	LightPosition = new QTreeWidgetItem(QStringList() << u8"光源位置");
	m_ScenePara->addTopLevelItem(LightPosition);
	LightPositionX = new QTreeWidgetItem(QStringList() << "X" << QString::number(parameter::lightPosition.x()));
	LightPosition->addChild(LightPositionX);
	LightPositionX->setData(1, Qt::UserRole, QLINEEDIT);
	LightPositionY = new QTreeWidgetItem(QStringList() << "Y" << QString::number(parameter::lightPosition.y()));
	LightPosition->addChild(LightPositionY);
	LightPositionY->setData(1, Qt::UserRole, QLINEEDIT);
	LightPositionZ = new QTreeWidgetItem(QStringList() << "Z" << QString::number(parameter::lightPosition.z()));
	LightPosition->addChild(LightPositionZ);
	LightPositionZ->setData(1, Qt::UserRole, QLINEEDIT);

	targetView = new QTreeWidgetItem(QStringList() << u8"视点位置");
	m_ScenePara->addTopLevelItem(targetView);
	targetViewX = new QTreeWidgetItem(QStringList() << "X" << QString::number(parameter::viewDirection.x()));
	targetView->addChild(targetViewX);
	targetViewX->setData(1, Qt::UserRole, QLINEEDIT);
	targetViewY = new QTreeWidgetItem(QStringList() << "Y" << QString::number(parameter::viewDirection.y()));
	targetView->addChild(targetViewY);
	targetViewY->setData(1, Qt::UserRole, QLINEEDIT);
	targetViewZ = new QTreeWidgetItem(QStringList() << "Z" << QString::number(parameter::viewDirection.z()));
	targetView->addChild(targetViewZ);
	targetViewZ->setData(1, Qt::UserRole, QLINEEDIT);

	m_ScenePara->setItemsExpandable(true);
	m_ScenePara->expandAll();

	//点击数据时候变成编辑模式
	connect(m_ScenePara, &QTreeWidget::itemPressed, this, [&](QTreeWidgetItem *item, int column) {
		if (1 == column)
		{
			if (item->treeWidget() != NULL)
			{
				{
					filter = true;//拦截opengl窗口中的键盘监听事件
					if (QDOUBLESPINBOX == item->data(1, Qt::UserRole))
					{
						QDoubleSpinBox *spinEditor = new QDoubleSpinBox();
						double value = item->text(1).toDouble();
						spinEditor->setValue(value);
						spinEditor->setDecimals(1);
						spinEditor->selectAll();
						m_ScenePara->setItemWidget(item, 1, spinEditor);
					}
					else if (QLINEEDIT == item->data(1, Qt::UserRole))
					{
						QLineEdit *lineEdit = new QLineEdit();
						QString text = item->text(1);
						lineEdit->setText(text);
						lineEdit->selectAll();
						m_ScenePara->setItemWidget(item, 1, lineEdit);
					}
				}
				item->treeWidget()->setFocus();
				isSceneedit = true;
			}
		}
		});

	//点击其他项时保存数据
	connect(m_ScenePara, &QTreeWidget::currentItemChanged, this, [&](QTreeWidgetItem *current, QTreeWidgetItem *previous) {
		if (current == previous || previous == nullptr || !isSceneedit)
		{
			return;
		}
		else {
			filter = false;//恢复opengl窗口中的键盘监听事件
			if (QDOUBLESPINBOX == previous->data(1, Qt::UserRole))
			{
				QDoubleSpinBox *spinEditor = (QDoubleSpinBox*)(m_ScenePara->itemWidget(previous, 1));
				double value = spinEditor->value();
				m_ScenePara->removeItemWidget(previous, 1);
				previous->setText(1, QString::number(value));
				delete spinEditor;
			}
			else if (QLINEEDIT == previous->data(1, Qt::UserRole))
			{
				QLineEdit *lineEdit = (QLineEdit*)(m_ScenePara->itemWidget(previous, 1));
				QString value = lineEdit->text();
				m_ScenePara->removeItemWidget(previous, 1);
				previous->setText(1, value);
				delete lineEdit;
			}
			m_ScenePara->update();
			isSceneedit = false;
		}
		});

	m_ScenePara->setContextMenuPolicy(Qt::CustomContextMenu);
	Scene_menu = new QMenu(m_ScenePara);
	QAction *redraw = Scene_menu->addAction(u8"更新");
	
	connect(m_ScenePara, &QTreeWidget::customContextMenuRequested, this, [&](const QPoint &pos) {
		Scene_menu->exec(QCursor::pos());
	});
	connect(redraw, SIGNAL(triggered()), this, SLOT(updateScene()));
	connect(qopengl, &m_qopenglwidget::updateViewpoint, this, [=](QVector3D p) {
		targetViewX->setText(1,QString::number(p.x()));
		targetViewY->setText(1, QString::number(p.y()));
		targetViewZ->setText(1, QString::number(p.z()));
		});
}

void mMainWindow::appendTree(QString name)
{
	QTreeWidgetItem *it = new QTreeWidgetItem();
	it->setText(0, name);
	it->setCheckState(0, Qt::Checked);
	ModelTree->addTopLevelItem(it);
}

void mMainWindow::LoadModel()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, QStringLiteral("选择三维模型文件！"), QString::fromStdString(parameter::path) + "data/", QStringLiteral("All files(*.*)"));
	for (QString meshPath : fileNames) {
		int res = QMessageBox::question(
			this,
			"",
			u8"即将加载"+ meshPath + u8"文件，是否自适应至窗口比例？",
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok
		);
		switch (res) {
		case QMessageBox::Ok:
			qopengl->LoadMesh(meshPath,true);
			break;
		case QMessageBox::Cancel:
			qopengl->LoadMesh(meshPath);
			break;
		default:
			break;
		}
		
	}
}

void mMainWindow::passmesh(std::vector<Mesh> meshes) {
	qopengl->handleMeshes(meshes);
}

void mMainWindow::DeleteModel()
{
	meshNode* node = item2node(selectedItem);
	node->destory();
	qopengl->nodeMap.erase(selectedItem->text(0));
	delete selectedItem;
	selectedItem = nullptr;
	qopengl->update();
}

void mMainWindow::updateScene()
{
	parameter::lightPosition.setX(LightPositionX->text(1).toDouble());
	parameter::lightPosition.setY(LightPositionY->text(1).toDouble());
	parameter::lightPosition.setZ(LightPositionZ->text(1).toDouble());

	parameter::viewDirection.setX(targetViewX->text(1).toDouble());
	parameter::viewDirection.setY(targetViewY->text(1).toDouble());
	parameter::viewDirection.setZ(targetViewZ->text(1).toDouble());

	qopengl->update();
}

Mesh * mMainWindow::item2mesh(QTreeWidgetItem * item)
{
	for (auto it = qopengl->meshMap.begin(); it != qopengl->meshMap.end(); it++) {
		if (item->text(0) == it->first) {
			return &(it->second);
		}
	}
	return nullptr;
}

meshNode * mMainWindow::item2node(QTreeWidgetItem * item)
{
	if (item) {
		for (auto it = qopengl->nodeMap.begin(); it != qopengl->nodeMap.end(); it++) {
			if (item->text(0) == it->first) {
				return it->second;
			}
		}
	}
	return nullptr;
}

void mMainWindow::initModelShow()
{
	qopengl = new m_qopenglwidget();
	this->setCentralWidget(qopengl);
}

void mMainWindow::initModelAttribute()
{
	m_AttibuteWidget = new QDockWidget(this);
	m_AttibuteWidget->setWindowTitle(u8"模型参数");
	m_AttibuteWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	m_AttibuteWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	this->addDockWidget(Qt::RightDockWidgetArea, m_AttibuteWidget);


	m_Attibute = new QTreeWidget();
	m_Attibute->setHeaderLabels(QStringList() << u8"属性"<< u8"值");
	m_Attibute->expandAll();

	ModelInformation = new QTreeWidgetItem(QStringList() << u8"模型信息");
	m_Attibute->addTopLevelItem(ModelInformation);

	ModelName = new QTreeWidgetItem(QStringList() << u8"名称" << "");
	ModelInformation->addChild(ModelName);
	ModelName->setData(1, Qt::UserRole, QLINEEDIT);

	RootPath = new QTreeWidgetItem(QStringList() << u8"数据源" << "");
	ModelInformation->addChild(RootPath);

	transform = new QTreeWidgetItem(QStringList() << u8"模型变换");
	m_Attibute->addTopLevelItem(transform);

	centerPosition = new QTreeWidgetItem(QStringList() << u8"变换中心");
	transform->addChild(centerPosition);

	Position = new QTreeWidgetItem(QStringList() << u8"空间坐标");
	transform->addChild(Position);
	PositionX = new QTreeWidgetItem(QStringList() << "X" << "");
	Position->addChild(PositionX);
	PositionX->setData(1, Qt::UserRole, QLINEEDIT);
	PositionY = new QTreeWidgetItem(QStringList() << "Y" << "");
	Position->addChild(PositionY);
	PositionY->setData(1, Qt::UserRole, QLINEEDIT);
	PositionZ = new QTreeWidgetItem(QStringList() << "Z" << "");
	Position->addChild(PositionZ);
	PositionZ->setData(1, Qt::UserRole, QLINEEDIT);

	Roatation = new QTreeWidgetItem(QStringList() << u8"绕轴旋转");
	transform->addChild(Roatation);
	RoatationX = new QTreeWidgetItem(QStringList() << u8"绕X轴" );
	Roatation->addChild(RoatationX);
	RoatationX->setData(1, Qt::UserRole, QLINEEDIT);
	RoatationY = new QTreeWidgetItem(QStringList() << u8"绕Y轴");
	Roatation->addChild(RoatationY);
	RoatationY->setData(1, Qt::UserRole, QLINEEDIT);
	RoatationZ = new QTreeWidgetItem(QStringList() << u8"绕Z轴");
	Roatation->addChild(RoatationZ);
	RoatationZ->setData(1, Qt::UserRole, QLINEEDIT);

	Scale = new QTreeWidgetItem(QStringList() << u8"缩放比例");
	transform->addChild(Scale);
	ScaleX = new QTreeWidgetItem(QStringList() << u8"X方向" );
	Scale->addChild(ScaleX);
	ScaleX->setData(1, Qt::UserRole, QDOUBLESPINBOX);
	ScaleY = new QTreeWidgetItem(QStringList() << u8"Y方向" );
	Scale->addChild(ScaleY);
	ScaleY->setData(1, Qt::UserRole, QDOUBLESPINBOX);
	ScaleZ = new QTreeWidgetItem(QStringList() << u8"Z方向" );
	Scale->addChild(ScaleZ);
	ScaleZ->setData(1, Qt::UserRole, QDOUBLESPINBOX);

	meshColor = new QTreeWidgetItem(QStringList() << u8"模型颜色");
	colorpicker = new ColorPicker();
	transform->addChild(meshColor);
	m_Attibute->setItemWidget(meshColor, 1, colorpicker);

	//点击数据时候变成编辑模式
	connect(m_Attibute, &QTreeWidget::itemPressed, this, [&](QTreeWidgetItem *item, int column) {
		if (1 == column)
		{
			filter = true;//拦截opengl窗口中的键盘监听事件
			if (item->treeWidget() != NULL)
			{
				{
					if (QDOUBLESPINBOX == item->data(1, Qt::UserRole))
					{
						QDoubleSpinBox *spinEditor = new QDoubleSpinBox();
						double value = item->text(1).toDouble();
						spinEditor->setValue(value);
						spinEditor->setDecimals(5);
						spinEditor->selectAll();
						m_Attibute->setItemWidget(item, 1, spinEditor);
					}
					else if (QLINEEDIT == item->data(1, Qt::UserRole))
					{
						QLineEdit *lineEdit = new QLineEdit();
						QString text = item->text(1);
						lineEdit->setText(text);
						lineEdit->selectAll();
						m_Attibute->setItemWidget(item, 1, lineEdit);
					}
				}
				item->treeWidget()->setFocus();
				isAttributeedit = true;
			}
		}
		});

	//点击其他项时保存数据
	connect(m_Attibute, &QTreeWidget::currentItemChanged, this, [&](QTreeWidgetItem *current, QTreeWidgetItem *previous) {
		if (current == previous || previous == nullptr||!isAttributeedit)
		{
			return;
		}
		else {
			filter = false;//恢复opengl窗口中的键盘监听事件
			if (QDOUBLESPINBOX == previous->data(1, Qt::UserRole))
			{
				QDoubleSpinBox *spinEditor = (QDoubleSpinBox*)(m_Attibute->itemWidget(previous, 1));
				double value = spinEditor->value();
				m_Attibute->removeItemWidget(previous, 1);
				previous->setText(1,QString::number(value));
				delete spinEditor;
			}
			else if (QLINEEDIT == previous->data(1, Qt::UserRole))
			{
				QLineEdit *lineEdit = (QLineEdit*)(m_Attibute->itemWidget(previous, 1));
				QString value = lineEdit->text();
				m_Attibute->removeItemWidget(previous, 1);
				previous->setText(1, value);
				delete lineEdit;
			}
			m_Attibute->update();
			isAttributeedit = false;
		}
		});

	m_AttibuteWidget->setWidget(m_Attibute);
	m_Attibute->setItemsExpandable(true);
	m_Attibute->expandAll();
	
	m_Attibute->setContextMenuPolicy(Qt::CustomContextMenu);
	Attibute_menu = new QMenu(m_Attibute);
	QAction *ReloadMesh = Attibute_menu->addAction(u8"更新");

	connect(m_Attibute, &QTreeWidget::customContextMenuRequested, this, [&](const QPoint &pos) {
		Attibute_menu->exec(QCursor::pos());
	});
	connect(ReloadMesh, SIGNAL(triggered()), this, SLOT(updateModel()));


	
}

void mMainWindow::initCombinationAttribute()
{

	m_CombinationAttibuteWidget = new QDockWidget(this);
	m_CombinationAttibuteWidget->hide();
	m_CombinationAttibuteWidget->setWindowTitle(u8"组合模型");
	m_CombinationAttibuteWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	m_CombinationAttibuteWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	this->addDockWidget(Qt::RightDockWidgetArea, m_CombinationAttibuteWidget);
	tabifyDockWidget(m_AttibuteWidget, m_CombinationAttibuteWidget);

	m_CombinationAttibute = new QTreeWidget();
	m_CombinationAttibute->setHeaderLabels(QStringList() << u8"属性" << u8"值");
	m_CombinationAttibute->expandAll();

	Children = new QTreeWidgetItem(QStringList() << u8"包含部件");
	m_CombinationAttibute->addTopLevelItem(Children);

	combinationPosition = new QTreeWidgetItem(QStringList() << u8"空间坐标");
	m_CombinationAttibute->addTopLevelItem(combinationPosition);
	combinationPositionX = new QTreeWidgetItem(QStringList() << "X" << "");
	combinationPosition->addChild(combinationPositionX);
	combinationPositionX->setData(1, Qt::UserRole, QLINEEDIT);
	combinationPositionY = new QTreeWidgetItem(QStringList() << "Y" << "");
	combinationPosition->addChild(combinationPositionY);
	combinationPositionY->setData(1, Qt::UserRole, QLINEEDIT);
	combinationPositionZ = new QTreeWidgetItem(QStringList() << "Z" << "");
	combinationPosition->addChild(combinationPositionZ);
	combinationPositionZ->setData(1, Qt::UserRole, QLINEEDIT);

	combinationRoatation = new QTreeWidgetItem(QStringList() << u8"绕轴旋转");
	m_CombinationAttibute->addTopLevelItem(combinationRoatation);
	combinationRoatationX = new QTreeWidgetItem(QStringList() << u8"绕X轴" << "0");
	combinationRoatation->addChild(combinationRoatationX);
	combinationRoatationX->setData(1, Qt::UserRole, QLINEEDIT);
	combinationRoatationY = new QTreeWidgetItem(QStringList() << u8"绕Y轴" << "0");
	combinationRoatation->addChild(combinationRoatationY);
	combinationRoatationY->setData(1, Qt::UserRole, QLINEEDIT);
	combinationRoatationZ = new QTreeWidgetItem(QStringList() << u8"绕Z轴" << "0");
	combinationRoatation->addChild(combinationRoatationZ);
	combinationRoatationZ->setData(1, Qt::UserRole, QLINEEDIT);

	combinationScale = new QTreeWidgetItem(QStringList() << u8"缩放比例");
	m_CombinationAttibute->addTopLevelItem(combinationScale);
	combinationScaleX = new QTreeWidgetItem(QStringList() << u8"X方向" << "1");
	combinationScale->addChild(combinationScaleX);
	combinationScaleX->setData(1, Qt::UserRole, QDOUBLESPINBOX);
	combinationScaleY = new QTreeWidgetItem(QStringList() << u8"Y方向" << "1");
	combinationScale->addChild(combinationScaleY);
	combinationScaleY->setData(1, Qt::UserRole, QDOUBLESPINBOX);
	combinationScaleZ = new QTreeWidgetItem(QStringList() << u8"Z方向" << "1");
	combinationScale->addChild(combinationScaleZ);
	combinationScaleZ->setData(1, Qt::UserRole, QDOUBLESPINBOX);

	combinationColor = new QTreeWidgetItem(QStringList() << u8"模型颜色");
	combinationColorpicker = new ColorPicker();
	m_CombinationAttibute->addTopLevelItem(combinationColor);
	m_CombinationAttibute->setItemWidget(combinationColor, 1, combinationColorpicker);

	m_CombinationAttibuteWidget->setWidget(m_CombinationAttibute);
	m_CombinationAttibute->setItemsExpandable(true);
	m_CombinationAttibute->expandAll();


	m_CombinationAttibute->setContextMenuPolicy(Qt::CustomContextMenu);
	m_CombinationAttibute_menu = new QMenu(m_CombinationAttibute);
	QAction *ReloadMesh = m_CombinationAttibute_menu->addAction(u8"更新");

	//点击数据时候变成编辑模式
	connect(m_CombinationAttibute, &QTreeWidget::itemPressed, this, [&](QTreeWidgetItem *item, int column) {
		if (1 == column)
		{
			if (item->treeWidget() != NULL)
			{
				{
					filter = true;//拦截opengl窗口中的键盘监听事件
					if (QDOUBLESPINBOX == item->data(1, Qt::UserRole))
					{
						QDoubleSpinBox *spinEditor = new QDoubleSpinBox();
						double value = item->text(1).toDouble();
						spinEditor->setValue(value);
						spinEditor->setDecimals(5);
						spinEditor->selectAll();
						m_CombinationAttibute->setItemWidget(item, 1, spinEditor);
					}
					else if (QLINEEDIT == item->data(1, Qt::UserRole))
					{
						QLineEdit *lineEdit = new QLineEdit();
						QString text = item->text(1);
						lineEdit->setText(text);
						lineEdit->selectAll();
						m_CombinationAttibute->setItemWidget(item, 1, lineEdit);
					}
				}
				item->treeWidget()->setFocus();
				isAttributeedit = true;
			}
		}
		});

	//点击其他项时保存数据
	connect(m_CombinationAttibute, &QTreeWidget::currentItemChanged, this, [&](QTreeWidgetItem *current, QTreeWidgetItem *previous) {
		if (current == previous || previous == nullptr || !isAttributeedit)
		{
			return;
		}
		else {
			filter = true;//恢复opengl窗口中的键盘监听事件
			if (QDOUBLESPINBOX == previous->data(1, Qt::UserRole))
			{
				QDoubleSpinBox *spinEditor = (QDoubleSpinBox*)(m_CombinationAttibute->itemWidget(previous, 1));
				double value = spinEditor->value();
				m_CombinationAttibute->removeItemWidget(previous, 1);
				previous->setText(1, QString::number(value));
				delete spinEditor;
			}
			else if (QLINEEDIT == previous->data(1, Qt::UserRole))
			{
				QLineEdit *lineEdit = (QLineEdit*)(m_CombinationAttibute->itemWidget(previous, 1));
				QString value = lineEdit->text();
				m_CombinationAttibute->removeItemWidget(previous, 1);
				previous->setText(1, value);
				delete lineEdit;
			}
			m_CombinationAttibute->update();
			isAttributeedit = false;
		}
		});

	connect(m_CombinationAttibute, &QTreeWidget::customContextMenuRequested, this, [&](const QPoint &pos) {
		m_CombinationAttibute_menu->exec(QCursor::pos());
		});
	connect(ReloadMesh, SIGNAL(triggered()), this, SLOT(updateCombineModel()));
}

void mMainWindow::getCombinationAttribute(QTreeWidgetItem * item)
{
	combinationPositionX->setText(1, item ? QString::number(qopengl->compositeCube->center.x()) : "");
	combinationPositionY->setText(1, item ? QString::number(qopengl->compositeCube->center.y()) : "");
	combinationPositionZ->setText(1, item ? QString::number(qopengl->compositeCube->center.z()) : "");

	deletesubmodel(Children);
	if (item) {
		Children = new QTreeWidgetItem(QStringList() << u8"包含部件");
		m_CombinationAttibute->addTopLevelItem(Children);
		itemmapmodel.clear();
		getsubmodel(item, Children);
	}
}
