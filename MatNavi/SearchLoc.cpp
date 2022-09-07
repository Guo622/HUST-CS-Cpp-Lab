#include "SearchLoc.h"
#include "MatNavi.h"
#include <QCompleter>

extern Navi* navi;

SearchLoc::SearchLoc(QMainWindow* p, QWidget* parent)
	: QWidget(parent), begin({ -1,-1 }), end({ -1,-1 })
{
	ui.setupUi(this);
	connect(ui.pushButtonQuit, SIGNAL(clicked()), this, SLOT(close()), Qt::UniqueConnection);
	connect(ui.pushButtonDone, SIGNAL(clicked()), this, SLOT(getstr()), Qt::UniqueConnection);
	connect(ui.pushButtonDone, SIGNAL(clicked()), p, SLOT(showPath()), Qt::UniqueConnection);
	if (navi) {//��������ģ��ƥ��Ĵʱ�
		for (int i = 0; i < navi->on; i++)
			wordList << navi->orgs[i].loc();
	}
	QCompleter* MyInfor = new QCompleter(wordList);  //�Զ����
	MyInfor->setFilterMode(Qt::MatchContains);
	ui.lineEditStart->setCompleter(MyInfor);
	ui.lineEditEnd->setCompleter(MyInfor);
}

SearchLoc::~SearchLoc() {}

void SearchLoc::getstr() { //��ȡ����
	ui.labelHit->setText("");
	QString s = ui.lineEditStart->text();
	QString e = ui.lineEditEnd->text();
	if (s.isEmpty() || e.isEmpty()) {  //����Ϊ��
		ui.labelHit->setStyleSheet("color: rgb(255, 0, 0)");
		ui.labelHit->setText(QString::fromLocal8Bit("������ʾ��������������յ㣡")); //����fromLocal8Bit��ʾ����
		return;
	}
	for (int i = 0; i < navi->on; i++) {  //��������ĵص�
		Organization& o = navi->orgs[i];
		if (s == o.loc()) { begin.setX(o.x()); begin.setY(o.y()); }
		if (e == o.loc()) { end.setX(o.x()); end.setY(o.y()); }
	}
	if (begin.x() == -1 || end.x() == -1) {  //����ʧ��
		ui.labelHit->setStyleSheet("color: rgb(255, 0, 0)");
		ui.labelHit->setText(QString::fromLocal8Bit("������ʾ������ȷ����������յ㣡")); //����fromLocal8Bit��ʾ����
		return;
	}
	ui.labelHit->setText("");
	close();
}



