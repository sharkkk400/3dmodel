#pragma once
#include "headers.h"
#include "m_qopenglwidget.h"
#include "ColorSelect.h"
#include "modelCube.h"
#include "meshNode.h"
#include "workclass.h"
class mMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit mMainWindow(QMainWindow* parent = nullptr);		//构造函数
	~mMainWindow();

	bool filter = false;
	void keyReleaseEvent(QKeyEvent* event) override;

	void initMenuBar();											//初始化菜单栏
	QMenuBar *m_menuBar;
	
	QThread loadThread;
	workclass *loader;
	void initModelTree();										//初始化模型树
	void setCheckStatus(QTreeWidgetItem* item,Qt::CheckState s);
	QDockWidget* m_DockWidget;
	QTreeWidget* ModelTree;
	QMenu* itemMenu;
	QMenu* blankMenu;
	QTreeWidgetItem *selectedItem = nullptr;
	Mesh* item2mesh(QTreeWidgetItem * item);
	meshNode* item2node(QTreeWidgetItem * item);
	void updatesubnode(QTreeWidgetItem * item, meshNode* parent);
	

	void initModelShow();										//初始化模型展示区域
	m_qopenglwidget* qopengl;


	void initModelAttribute();									//初始化模型属性
	QDockWidget* m_AttibuteWidget;
	QTreeWidget* m_Attibute;
	QMenu* Attibute_menu;
	QTreeWidgetItem *ModelInformation;
	QTreeWidgetItem *ModelName;
	QTreeWidgetItem *RootPath;
	QTreeWidgetItem *transform;
	QTreeWidgetItem *centerPosition;
	QTreeWidgetItem *Position;
	QTreeWidgetItem *PositionX;
	QTreeWidgetItem *PositionY;
	QTreeWidgetItem *PositionZ;
	QTreeWidgetItem *Roatation;
	QTreeWidgetItem *RoatationX;
	QTreeWidgetItem *RoatationY;
	QTreeWidgetItem *RoatationZ;
	QTreeWidgetItem *Scale;
	QTreeWidgetItem *ScaleX;
	QTreeWidgetItem *ScaleY;
	QTreeWidgetItem *ScaleZ;
	QTreeWidgetItem *meshColor;
	ColorPicker* colorpicker;
	bool isAttributeedit = false;
		
	void getModelAttribute(QTreeWidgetItem * item);
	void getsubmodel(QTreeWidgetItem * item, QTreeWidgetItem * parent);
	void deletesubmodel(QTreeWidgetItem * item);
	QList<Mesh*> meshlist;
	void getselecetedmesh(QTreeWidgetItem * item);
	std::map<QString, QTreeWidgetItem*> itemmapmodel;

	void initCombinationAttribute();
	void getCombinationAttribute(QTreeWidgetItem * item);
	QDockWidget* m_CombinationAttibuteWidget;
	QTreeWidget* m_CombinationAttibute;
	QTreeWidgetItem *combinationPosition;
	QTreeWidgetItem *combinationPositionX;
	QTreeWidgetItem *combinationPositionY;
	QTreeWidgetItem *combinationPositionZ;
	QTreeWidgetItem *combinationRoatation;
	QTreeWidgetItem *combinationRoatationX;
	QTreeWidgetItem *combinationRoatationY;
	QTreeWidgetItem *combinationRoatationZ;
	QTreeWidgetItem *combinationScale;
	QTreeWidgetItem *combinationScaleX;
	QTreeWidgetItem *combinationScaleY;
	QTreeWidgetItem *combinationScaleZ;
	QTreeWidgetItem *combinationColor;
	ColorPicker* combinationColorpicker;
	QTreeWidgetItem *Children;
	QMenu* m_CombinationAttibute_menu;

	//场景参数
	void initSceneParaWidget();
	QDockWidget* m_SceneParaWidget;
	QTreeWidget* m_ScenePara;
	QMenu* Scene_menu;
	QTreeWidgetItem* skyPara;
	QCheckBox* drawsky;
	QTreeWidgetItem* groundPara;
	QCheckBox* drawground;
	QTreeWidgetItem* AixsPara;
	QCheckBox* drawAixs;
	QTreeWidgetItem* StatusPara;
	QCheckBox* drawStatus;
	QTreeWidgetItem* LightPara;
	QCheckBox* drawLight;
	QTreeWidgetItem* LightPosition;
	QTreeWidgetItem* LightPositionX;
	QTreeWidgetItem* LightPositionY;
	QTreeWidgetItem* LightPositionZ;
	QTreeWidgetItem* targetView;
	QTreeWidgetItem* targetViewX;
	QTreeWidgetItem* targetViewY;
	QTreeWidgetItem* targetViewZ;

	bool isSceneedit = false;

signals:
	void beginLoad(QStringList pathlist);

public Q_SLOTS:
	void slotClicked(QTreeWidgetItem *item, int column);
	void appendTree(QString name);
	void LoadModel();
	void DeleteModel();
	void updateScene();
	void updateModel();
	void updateCombineModel();
	void updatemeshNode();
	void passmesh(std::vector<Mesh> meshes);

};

