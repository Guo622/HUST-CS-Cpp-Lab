#pragma once
#ifndef FILELOAD_H
#define FILELOAD_H


#include <QWidget>
#include "ui_FileLoad.h"

class FileLoad : public QWidget    //�ļ��������
{
	Q_OBJECT

public:
	FileLoad(QWidget* parent = Q_NULLPTR);
	~FileLoad();
	QGraphicsView* par;
	void myShow(QGraphicsView* p);

private:
	Ui::FileLoad ui;

private slots:
	void inputStop();     //����վ���ļ�
	void inputLine();     //������·�ļ�
	void inputOrg();	  //������֯�ļ�
	void checkFile();	  //����ļ�·��
};

#endif 