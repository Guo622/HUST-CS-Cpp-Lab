#pragma once
#ifndef SEARCHLOC_H
#define SEARCHLOC_H

#include <QtWidgets/QMainWindow>
#include <QWidget>
#include <QString>
#include "ui_SearchLoc.h"

class SearchLoc : public QWidget  //ģ��ƥ�������ص����
{
	Q_OBJECT

public:
	QPoint begin;	//��ʼ�ص�
	QPoint end;		//��ֹ�ص�
	SearchLoc(QMainWindow* p, QWidget* parent = Q_NULLPTR);
	~SearchLoc();

private:
	Ui::SearchLoc ui;
	QStringList wordList;   //����ģ��ƥ��Ĵʱ�
private slots:
	void getstr();   //�������ĵص�
};

#endif