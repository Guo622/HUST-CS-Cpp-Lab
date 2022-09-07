#pragma once
#ifndef FILELOAD_H
#define FILELOAD_H


#include <QWidget>
#include "ui_FileLoad.h"

class FileLoad : public QWidget    //文件输入界面
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
	void inputStop();     //输入站点文件
	void inputLine();     //输入线路文件
	void inputOrg();	  //输入组织文件
	void checkFile();	  //检查文件路径
};

#endif 