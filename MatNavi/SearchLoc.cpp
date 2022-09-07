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
	if (navi) {//构建用于模糊匹配的词表
		for (int i = 0; i < navi->on; i++)
			wordList << navi->orgs[i].loc();
	}
	QCompleter* MyInfor = new QCompleter(wordList);  //自动填充
	MyInfor->setFilterMode(Qt::MatchContains);
	ui.lineEditStart->setCompleter(MyInfor);
	ui.lineEditEnd->setCompleter(MyInfor);
}

SearchLoc::~SearchLoc() {}

void SearchLoc::getstr() { //获取输入
	ui.labelHit->setText("");
	QString s = ui.lineEditStart->text();
	QString e = ui.lineEditEnd->text();
	if (s.isEmpty() || e.isEmpty()) {  //输入为空
		ui.labelHit->setStyleSheet("color: rgb(255, 0, 0)");
		ui.labelHit->setText(QString::fromLocal8Bit("操作提示：请输入起点与终点！")); //不用fromLocal8Bit显示乱码
		return;
	}
	for (int i = 0; i < navi->on; i++) {  //查找输入的地点
		Organization& o = navi->orgs[i];
		if (s == o.loc()) { begin.setX(o.x()); begin.setY(o.y()); }
		if (e == o.loc()) { end.setX(o.x()); end.setY(o.y()); }
	}
	if (begin.x() == -1 || end.x() == -1) {  //查找失败
		ui.labelHit->setStyleSheet("color: rgb(255, 0, 0)");
		ui.labelHit->setText(QString::fromLocal8Bit("操作提示：请正确输入起点与终点！")); //不用fromLocal8Bit显示乱码
		return;
	}
	ui.labelHit->setText("");
	close();
}



