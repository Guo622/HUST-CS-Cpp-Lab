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
	FileLoad* fl;		//�ļ���ȡ����
	SearchLoc* src;		//�����ص㴰��
	QTimer* myTimer;
	QGraphicsItemGroup* gItem;
	void deleteItems();
private slots:
	void minChange();	//����ת��
	void loadmap();		//���ص�ͼ
	void closewnd();	//�رմ���
	void seachLoc();	//�����ص�
	void showPath();	//��ʾ��·
};

//�����Լ��ĳ���MyScene���Ա��ܹ�������������¼�
class MyScene :public QGraphicsScene {
public:
	explicit MyScene(QObject* parent = 0);
	void stopLines(QGraphicsView*);
protected:
	QGraphicsView* qgv;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event);	//����mousePressEvent�Բ�������¼�
	void mousePressEvent(QGraphicsSceneMouseEvent* event);	//����mousePressEvent�Բ�������¼�
};

class MyItem : public QGraphicsRectItem {
private:
	int cx, cy;			//���ʱ������
	int cf;				//������=1����ʾ�ص㣬�Ҽ����=2��ʾ�յ�
	int cs;				//�����õ���������
	vector<int> bs;		//���վ���վ����
public:
	MyItem(int x, int y, int f);
	MyItem& operator<<(int s);
	int operator()(int x, int y);
	int& x();
	int& y();
	int& f();
	int& s();
	int& operator[](int);
	int checkAllStops();	//��������վ��
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif
