#include "ViewTip.h"

ViewTip::ViewTip(const QString& msg, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);
	initFrame(msg);
	myTimer = new QTimer(this);
	myTimer->setSingleShot(true);  //定时器只执行一次
	connect(myTimer, &QTimer::timeout, this, [=]() {this->close(); });
}

ViewTip::~ViewTip() {
	if (this->myTimer) this->myTimer->deleteLater();
}

void ViewTip::startTimer(int ms) { this->myTimer->start(ms); }

void ViewTip::initFrame(const QString& msg) { ui.labelMsg->setText(msg); }
