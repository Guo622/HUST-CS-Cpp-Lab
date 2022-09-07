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

void FileLoad::inputStop() {//����վ���ļ�
	ui.labelHits->setText("");
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("*.txt"));
	ui.textEditStop->setText(fname);
}

void FileLoad::inputLine() {//������·�ļ�
	ui.labelHits->setText("");
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("*.txt"));
	ui.textEditLine->setText(fname);
}

void FileLoad::inputOrg() {//������֯�ļ�
	ui.labelHits->setText("");
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("*.txt"));
	ui.textEditOrg->setText(fname);
}


void FileLoad::checkFile() {//����ļ�·��
	QString fs = ui.textEditStop->toPlainText();
	QString fl = ui.textEditLine->toPlainText();
	QString fo = ui.textEditOrg->toPlainText();
	if (fs.isEmpty() || fl.isEmpty() || fo.isEmpty()) { //����Ϊ��
		ui.labelHits->setStyleSheet("color: rgb(255,0,0)");
		ui.labelHits->setText(QString::fromLocal8Bit("�ļ�·������"));
		ui.textEditStop->setFocus();
		return;
	}
	try {	//����������Ϣ��navi
		if (navi)	delete navi;
		navi = new Navi(fs.toStdString().c_str(), fl.toStdString().c_str(), fo.toStdString().c_str());
		((MyScene*)(par->scene()))->stopLines(par); //�ڱ�����ͼ�ϻ���վ�㡢��·����֯
	}
	catch (...) {//����ʧ��
		navi = NULL;
		close();
		QMessageBox::information(NULL, QString::fromLocal8Bit("����"),
			QString::fromLocal8Bit("��ʼ��ʧ�ܣ�"));
	}
	ui.labelHits->setText("");
	close();
}
