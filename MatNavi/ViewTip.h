#pragma once
#ifndef VIEWTIP_H
#define VIEWTIP_H

#include <QDialog>
#include <QTimer>
#include "ui_ViewTip.h"

class ViewTip : public QDialog  //��ʾ�ص���Ϣ
{
	Q_OBJECT

public:
	ViewTip(const QString& msg, QWidget* parent = Q_NULLPTR);
	~ViewTip();
	void startTimer(int);

private:
	Ui::ViewTip ui;
	QTimer* myTimer;
	void initFrame(const QString& msg);
};
#endif 