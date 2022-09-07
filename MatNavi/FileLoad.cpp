#include "FileLoad.h"
#include "logiclayer.h"
#include "MatNavi.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

extern Navi* navi;

FileLoad::FileLoad(QWidget* parent)
	: QWidget(parent), par(Q_NULLPTR)
{
	ui.setupUi(this);
	connect(ui.pushButtonStop, SIGNAL(clicked()), this, SLOT(inputStop()), Qt::UniqueConnection);
	connect(ui.pushButtonLine, SIGNAL(clicked()), this, SLOT(inputLine()), Qt::UniqueConnection);
	connect(ui.pushButtonOrg, SIGNAL(clicked()), this, SLOT(inputOrg()), Qt::UniqueConnection);
	connect(ui.pushButtonDone, SIGNAL(clicked()), this, SLOT(checkFile()), Qt::UniqueConnection);
	connect(ui.pushButtonQuit, SIGNAL(clicked()), this, SLOT(close()), Qt::UniqueConnection);
}

FileLoad::~FileLoad() {}

void FileLoad::myShow(QGraphicsView* p) { par = p; show(); }

void FileLoad::inputStop() {//输入站点文件
	ui.labelHits->setText("");
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("*.txt"));
	ui.textEditStop->setText(fname);
}

void FileLoad::inputLine() {//输入线路文件
	ui.labelHits->setText("");
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("*.txt"));
	ui.textEditLine->setText(fname);
}

void FileLoad::inputOrg() {//输入组织文件
	ui.labelHits->setText("");
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("*.txt"));
	ui.textEditOrg->setText(fname);
}


void FileLoad::checkFile() {//检查文件路径
	QString fs = ui.textEditStop->toPlainText();
	QString fl = ui.textEditLine->toPlainText();
	QString fo = ui.textEditOrg->toPlainText();
	if (fs.isEmpty() || fl.isEmpty() || fo.isEmpty()) { //输入为空
		ui.labelHits->setStyleSheet("color: rgb(255,0,0)");
		ui.labelHits->setText(QString::fromLocal8Bit("文件路径错误！"));
		ui.textEditStop->setFocus();
		return;
	}
	try {	//构建基本信息类navi
		if (navi)	delete navi;
		navi = new Navi(fs.toStdString().c_str(), fl.toStdString().c_str(), fo.toStdString().c_str());
		((MyScene*)(par->scene()))->stopLines(par); //在背景地图上画出站点、线路、组织
	}
	catch (...) {//构建失败
		navi = NULL;
		close();
		QMessageBox::information(NULL, QString::fromLocal8Bit("警告"),
			QString::fromLocal8Bit("初始化失败！"));
	}
	ui.labelHits->setText("");
	close();
}
