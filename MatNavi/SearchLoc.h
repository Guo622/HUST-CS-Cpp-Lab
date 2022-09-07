#pragma once
#ifndef SEARCHLOC_H
#define SEARCHLOC_H

#include <QtWidgets/QMainWindow>
#include <QWidget>
#include <QString>
#include "ui_SearchLoc.h"

class SearchLoc : public QWidget  //模糊匹配搜索地点界面
{
	Q_OBJECT

public:
	QPoint begin;	//起始地点
	QPoint end;		//终止地点
	SearchLoc(QMainWindow* p, QWidget* parent = Q_NULLPTR);
	~SearchLoc();

private:
	Ui::SearchLoc ui;
	QStringList wordList;   //用于模糊匹配的词表
private slots:
	void getstr();   //获得输入的地点
};

#endif