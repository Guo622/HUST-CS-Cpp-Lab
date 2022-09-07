#pragma once
#ifndef MATNAVI_H
#define MATNAVI_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsRectItem>
#include "FileLoad.h"
#include "SearchLoc.h"
#include "logiclayer.h"
#include "ui_MatNavi.h"

class MatNavi : public QMainWindow
{
	Q_OBJECT
public:
	MatNavi(QWidget* parent = Q_NULLPTR);
	~MatNavi();
protected:
	void closeEvent(QCloseEvent* event);
private:
	Ui::MatNaviClass ui;
	FileLoad* fl;		//文件读取窗口
	SearchLoc* src;		//搜索地点窗口
	QTimer* myTimer;
	QGraphicsItemGroup* gItem;
	void deleteItems();
private slots:
	void minChange();	//最少转乘
	void loadmap();		//加载地图
	void closewnd();	//关闭窗口
	void seachLoc();	//搜索地点
	void showPath();	//显示线路
};

//定义自己的场景MyScene，以便能够捕获鼠标或键盘事件
class MyScene :public QGraphicsScene {
public:
	explicit MyScene(QObject* parent = 0);
	void stopLines(QGraphicsView*);
protected:
	QGraphicsView* qgv;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);	//覆盖mousePressEvent以捕获鼠标事件
	void mousePressEvent(QGraphicsSceneMouseEvent* event);	//覆盖mousePressEvent以捕获鼠标事件
};

class MyItem : public QGraphicsRectItem {
private:
	int cx, cy;			//点击时的坐标
	int cf;				//左键点击=1，表示地点，右键点击=2表示终点
	int cs;				//靠近该点的坐标个数
	vector<int> bs;		//存放站点的站点编号
public:
	MyItem(int x, int y, int f);
	MyItem& operator<<(int s);
	int operator()(int x, int y);
	int& x();
	int& y();
	int& f();
	int& s();
	int& operator[](int);
	int checkAllStops();	//检索所有站点
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif
