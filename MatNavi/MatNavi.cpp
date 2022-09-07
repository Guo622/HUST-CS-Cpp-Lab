#include "MatNavi.h"
#include "ViewTip.h"
#include "SearchLoc.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QString>
#include <cmath>
#include <iostream>
#include <QDebug>


Navi* navi = NULL;			//��ͼ��Ϣ
bool route_flag = false;	//�Ƿ��ѯ·��
bool depart_flag = false;	//�Ƿ�ѡȡ�������
bool arrive_flag = false;	//�Ƿ�ѡȡ�����յ�

MatNavi::MatNavi(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	fl = Q_NULLPTR;
	src = Q_NULLPTR;
	gItem = Q_NULLPTR;
	myTimer = new QTimer(this);
	myTimer->setSingleShot(this);    //��ʱ��ִֻ��һ��
	connect(ui.action_minChange, SIGNAL(triggered(bool)), this, SLOT(minChange()));
	connect(ui.action_open, SIGNAL(triggered(bool)), this, SLOT(loadmap()));
	connect(ui.action_exit, SIGNAL(triggered(bool)), this, SLOT(closewnd()));
	connect(ui.action_searchLoc, SIGNAL(triggered(bool)), this, SLOT(seachLoc()));
	connect(myTimer, &QTimer::timeout, this, [=]() {
		QList<QGraphicsItem*>listItem = ui.graphicsView->scene()->items();
		deleteItems();			//��ѯ�����ʾʱ��һ������ɾ������������ͼԪ
		route_flag = false;		//��ѯ�����ʾ��ϣ���������ѡȡ���������յ�
		});
}

MatNavi::~MatNavi() {
	if (fl) {
		fl->hide();	delete fl; fl = Q_NULLPTR;
		delete myTimer;	deleteItems(); delete navi;
	}
}

void MatNavi::closewnd() {
	if (ui.action_exit->isChecked() == false)	return;	//�����һ�δ������Σ��ڶ��δ���ֱ�ӷ���
	ui.action_exit->setChecked(false);	//����һ�δ�������״̬Ϊfalse,��ֹ��2�δ�������
	if (fl) { fl->hide(); delete fl; fl = Q_NULLPTR; }
	close();
}

void MatNavi::closeEvent(QCloseEvent* event) {
	if (fl) { fl->hide(); delete fl; fl = Q_NULLPTR; }
	if (src) { src->hide(); delete src, src = Q_NULLPTR; }
}

void MatNavi::loadmap() {	//���ص�ͼ
	if (ui.action_open->isChecked() == false)	return;	//������������Σ��ڶ��δ���ֱ�ӷ���
	ui.action_open->setChecked(false);	//����һ�δ�������״̬Ϊfalse,��ֹ��2�δ�������
	if (fl) { fl->show(); return; }		//�����ǰ�򿪹�վ�㼰��·���봰����ֱ����ʾ�ô���
	arrive_flag = depart_flag = false;	//��ʱδѡȡ���������յ�
	fl = new FileLoad();		//�����ǰû�д򿪹�վ�㼰��·���봰��
	fl->setWindowFlags(Qt::WindowStaysOnTopHint);	//�����������ʾ
	fl->myShow(ui.graphicsView);
}

void MatNavi::seachLoc() {		//�����ص�
	if (fl == Q_NULLPTR)	return;
	if (ui.action_searchLoc->isChecked() == false)	return;
	ui.action_searchLoc->setChecked(false);
	if (src) { src->show(); return; }
	src = new SearchLoc(this);
	src->setWindowFlags(Qt::WindowStaysOnTopHint);
	src->show();
}

void MatNavi::deleteItems() { //ɾ������������ͼԪ
	if (gItem == Q_NULLPTR)	return;
	ui.graphicsView->scene()->removeItem(gItem);
	for (int i = 0; i < gItem->childItems().size(); i++) {
		QGraphicsItem* item = (gItem->childItems())[i];
		gItem->removeFromGroup(item);
		delete item;
	}
	delete gItem;
	gItem = Q_NULLPTR;
}

void MatNavi::minChange() {
	//�ȼ�������ת�˵�·��,�Ȼ�����������յ�����
	if (ui.action_minChange->isChecked() == false)	return;	//�����һ�δ������Σ��ڶ��δ���ֱ�ӷ���
	ui.action_minChange->setChecked(false); //����һ�δ�������״̬Ϊfalse,��ֹ��2�δ�������
	QList<QGraphicsItem*> listItem;
	if (!(depart_flag && arrive_flag))	return; //��û��ѡ�в��������յ㣬�򷵻�
	listItem = ui.graphicsView->scene()->items();
	MyItem* Depart = (MyItem*)listItem[0];
	MyItem* Arrive = (MyItem*)listItem[1];
	if (Depart->f() != 1) {			//�����ǲ�����㣬�򽻻�
		Depart = (MyItem*)listItem[1];
		Arrive = (MyItem*)listItem[0];
	}
	//��ʼ�齨ͼԪ�飺������ʾת�˷�����·��
	QGraphicsEllipseItem* EllipseIte;
	QGraphicsLineItem* LineIte;
	MyScene* scene = (MyScene*)(ui.graphicsView->scene());
	QBrush brush(Qt::green);		//������ɫ
	QPen pens(brush, 6, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);//վ��ı�
	QPen penl(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);//��·�ı�
	route_flag = true;			//�����ѯ·����ʾ�ڼ䣬����Ӧ����������յ�ѡȡ
	int cnt, n = 0;				//cntΪ����ת�˷�������nΪ����ת�˴���
	vector<Route> r;			//���ؿ���ת�˷���
	for (int i = 0; i < Depart->s(); i++) {//�ӽ��������й���վ��
		int s = (*Depart)[i];			//������վ����s
		for (int j = 0; j < Arrive->s(); j++) {//�ӽ��յ�����й���վ��
			int t = (*Arrive)[j];			   //����յ�վ����t
			if (s == t) {					   //���վ���յ�վ��ͬ����ת��
				deleteItems();				   //ɾ����ǰ��ת�˷���·����ʾ
				gItem = new QGraphicsItemGroup();
				EllipseIte = new QGraphicsEllipseItem(Depart->x(), Depart->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(brush);
				gItem->addToGroup(EllipseIte);   //��������λ��
				LineIte = new QGraphicsLineItem(Depart->x(), Depart->y(), Arrive->x(), Arrive->y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);		 //�������յ��·��
				EllipseIte = new QGraphicsEllipseItem(Arrive->x(), Arrive->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(QBrush(QColor(255, 170, 0)));
				gItem->addToGroup(EllipseIte);	 //�����յ��λ��
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
				continue;
			}
			cnt = navi->minChange(s, t, n, r);	//�õ���ת�˷�����
			for (int k = 0; k < cnt; k++) {		 //���ڵ�K��ת�˷�����route=r[K], ת�˴�����Ϊn 
				deleteItems();
				gItem = new QGraphicsItemGroup();
				EllipseIte = new QGraphicsEllipseItem(Depart->x(), Depart->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(brush);
				gItem->addToGroup(EllipseIte);	 //��������λ��
				int fr = s, to = t;				 //��ʼվfr���յ�վto
				int fm, tm;						 //�ѳ���·fm��ת����·tm
				int begin, end;					 //��·�е���ʼվ�����begin,��ֹվ�����end
				LineIte = new QGraphicsLineItem(Depart->x(), Depart->y(),
					navi->stops[s].x(), navi->stops[s].y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);				 //�����վ	
				if (n == 1 && r[k][0].stop() == -1) {	 //����i·��i·(��ʱS()==-1)����ת��
					fm = r[k][0].from();				 //�ѳ���·���fm
					begin = navi->lines[fm].has(fr);	 //��ʼվ������·�е����
					end = navi->lines[fm].has(to);		 //��ֹվ������·�е����
					if (begin > end) { tm = begin; begin = end; end = tm; }
					for (int x = begin; x < end; x++) {	  //����ʼվ����һ��ŵ���ֹվ�����
						fr = navi->lines[fm][x];		  //�õ���վ����Ŷ�Ӧ��վ����
						to = navi->lines[fm][x + 1];	  //�õ���վ����Ŷ�Ӧ��վ����
						LineIte = new QGraphicsLineItem(navi->stops[fr].x(), navi->stops[fr].y(),
							navi->stops[to].x(), navi->stops[to].y());
						LineIte->setPen(penl);
						gItem->addToGroup(LineIte);       //����һվ��·��
					}
				}
				else {
					for (int x = 0; x < n; x++) {		 //����ÿ��ת��
						fm = r[k][x].from();			 //����ÿ��ת�˵���ʼ��·
						begin = navi->lines[fm].has(fr);
						to = r[k][x].stop();			 //������ʼ��·��ת��վ��
						end = navi->lines[fm].has(to);
						if (begin > end) { tm = begin; begin = end; end = tm; }
						for (int y = begin; y < end; y++) {		//����ʼվ����һ��ŵ���ֹվ�����
							int ff = navi->lines[fm][y];		//�õ���վ����Ŷ�Ӧ��վ����
							int tt = navi->lines[fm][y + 1];	//�õ���վ����Ŷ�Ӧ��վ����
							LineIte = new QGraphicsLineItem(navi->stops[ff].x(), navi->stops[ff].y(),
								navi->stops[tt].x(), navi->stops[tt].y());
							LineIte->setPen(penl);
							gItem->addToGroup(LineIte);			//����һվ��·��
						}
						fr = to;		//��Ϊ��һ���
					}
					fm = r[k][n - 1].to();		//��������������·
					begin = navi->lines[fm].has(fr);
					end = navi->lines[fm].has(t);
					if (begin > end) { tm = begin; begin = end; end = tm; }
					for (int x = begin; x < end; x++) {		 //����ʼվ����һ��ŵ���ֹվ�����
						int ff = navi->lines[fm][x];		//�õ���վ����Ŷ�Ӧ��վ����
						int tt = navi->lines[fm][x + 1];	//�õ���վ����Ŷ�Ӧ��վ����
						LineIte = new QGraphicsLineItem(navi->stops[ff].x(), navi->stops[ff].y(),
							navi->stops[tt].x(), navi->stops[tt].y());
						LineIte->setPen(penl);
						gItem->addToGroup(LineIte);			 //����һվ��·��
					}
				}
				LineIte = new QGraphicsLineItem(navi->stops[t].x(), navi->stops[t].y(), Arrive->x(), Arrive->y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);				 //�������յ��·��
				EllipseIte = new QGraphicsEllipseItem(Arrive->x(), Arrive->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(QBrush(QColor(255, 170, 0)));
				gItem->addToGroup(EllipseIte);			 //�����յ��λ��
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
			}
		}
	}
	this->myTimer->start(5000);			 //չʾ��ѯ��·�����5��
}
void MatNavi::showPath() {   //��ʾ��·
	if (src->begin.x() == src->end.x() && src->end.y() == src->begin.y())
		return;
	MyItem* Depart = new MyItem(src->begin.x(), src->begin.y(), 1);
	MyItem* Arrive = new MyItem(src->end.x(), src->end.y(), 2);
	QGraphicsEllipseItem* EllipseIte;
	QGraphicsLineItem* LineIte;
	MyScene* scene = (MyScene*)(ui.graphicsView->scene());
	QBrush brush(Qt::green);
	QPen pens(brush, 6, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	QPen penl(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	route_flag = true;
	int cnt, n = 0;
	vector<Route> r;
	for (int i = 0; i < Depart->s(); i++) {
		int s = (*Depart)[i];
		for (int j = 0; j < Arrive->s(); j++) {
			int t = (*Arrive)[j];
			if (s == t) {
				deleteItems();
				gItem = new QGraphicsItemGroup();
				EllipseIte = new QGraphicsEllipseItem(Depart->x(), Depart->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(brush);
				gItem->addToGroup(EllipseIte);
				LineIte = new QGraphicsLineItem(Depart->x(), Depart->y(), Arrive->x(), Arrive->y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);
				EllipseIte = new QGraphicsEllipseItem(Arrive->x(), Arrive->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(QBrush(QColor(255, 170, 0)));
				gItem->addToGroup(EllipseIte);
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
				continue;
			}
			cnt = navi->minChange(s, t, n, r);
			for (int k = 0; k < cnt; k++) {
				deleteItems();
				gItem = new QGraphicsItemGroup();
				EllipseIte = new QGraphicsEllipseItem(Depart->x(), Depart->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(brush);
				gItem->addToGroup(EllipseIte);
				int fr = s, to = t;
				int fm, tm;
				int begin, end;
				LineIte = new QGraphicsLineItem(Depart->x(), Depart->y(),
					navi->stops[s].x(), navi->stops[s].y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);
				if (n == 1 && r[k][0].stop() == -1) {
					fm = r[k][0].from();
					begin = navi->lines[fm].has(fr);
					end = navi->lines[fm].has(to);
					if (begin > end) { tm = begin; begin = end; end = tm; }
					for (int x = begin; x < end; x++) {
						fr = navi->lines[fm][x];
						to = navi->lines[fm][x + 1];
						LineIte = new QGraphicsLineItem(navi->stops[fr].x(), navi->stops[fr].y(),
							navi->stops[to].x(), navi->stops[to].y());
						LineIte->setPen(penl);
						gItem->addToGroup(LineIte);
					}
				}
				else {
					for (int x = 0; x < n; x++) {
						fm = r[k][x].from();
						begin = navi->lines[fm].has(fr);
						to = r[k][x].stop();
						end = navi->lines[fm].has(to);
						if (begin > end) { tm = begin; begin = end; end = tm; }
						for (int y = begin; y < end; y++) {
							int ff = navi->lines[fm][y];
							int tt = navi->lines[fm][y + 1];
							LineIte = new QGraphicsLineItem(navi->stops[ff].x(), navi->stops[ff].y(),
								navi->stops[tt].x(), navi->stops[tt].y());
							LineIte->setPen(penl);
							gItem->addToGroup(LineIte);
						}
						fr = to;
					}
					fm = r[k][n - 1].to();
					begin = navi->lines[fm].has(fr);
					end = navi->lines[fm].has(t);
					if (begin > end) { tm = begin; begin = end; end = tm; }
					for (int x = begin; x < end; x++) {
						int ff = navi->lines[fm][x];
						int tt = navi->lines[fm][x + 1];
						LineIte = new QGraphicsLineItem(navi->stops[ff].x(), navi->stops[ff].y(),
							navi->stops[tt].x(), navi->stops[tt].y());
						LineIte->setPen(penl);
						gItem->addToGroup(LineIte);
					}
				}
				LineIte = new QGraphicsLineItem(navi->stops[t].x(), navi->stops[t].y(), Arrive->x(), Arrive->y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);
				EllipseIte = new QGraphicsEllipseItem(Arrive->x(), Arrive->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(QBrush(QColor(255, 170, 0)));
				gItem->addToGroup(EllipseIte);
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
			}
		}
	}
	this->myTimer->start(5000);
}

MyScene::MyScene(QObject* parent) :QGraphicsScene(parent) {
	clearFocus();
	qgv = Q_NULLPTR; //û�м��ص�ͼ�ļ�ʱ
}

void MyScene::stopLines(QGraphicsView* parent) { //���ص�ͼվ�����·
	QSize viewsize = parent->size();	//ȡ��graphicsview��ͼ�����С
	MyScene* scene;
	if (parent->scene() != Q_NULLPTR)	delete parent->scene();
	scene = new MyScene(parent);		//����scene
	scene->setSceneRect(0, 0, viewsize.width(), viewsize.height());
	scene->qgv = parent;
	QBrush brush(Qt::red);   //������ɫ
	QPen pens(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);  //վ��ı�

	//��ʾ����վ��
	for (int i = 0; i < navi->sn; i++)
		scene->addEllipse(navi->stops[i].x(), navi->stops[i].y(), 6, 6, pens, brush);
	brush.setColor(Qt::blue);

	//��ʾ������·
	QPen penl(brush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);  //��·�ı�
	for (int i = 0; i < navi->ln; i++) {
		Line& line = navi->lines[i];
		int n = line.s();
		for (int j = 1; j < n; j++) {
			Stop& begin = navi->stops[line[j - 1]];
			Stop& end = navi->stops[line[j]];
			QLineF ql = QLineF(begin.x(), begin.y(), end.x(), end.y());
			scene->addLine(ql, penl);
		}
	}
	brush.setColor(Qt::black);

	//��ʾ������֯
	QPen peno(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //��֯�ı�
	for (int i = 0; i < navi->on; i++)
		scene->addEllipse(navi->orgs[i].x(), navi->orgs[i].y(), 6, 6, peno, brush);
	parent->setScene(scene);
	parent->show();
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	if (qgv == Q_NULLPTR) { //���û�м��ص�ͼ�ļ�
		QGraphicsScene::mouseMoveEvent(event);
		return;
	}
	QPointF point = event->scenePos();	//��ȡ����ƶ�ʱ������	
	for (int i = 0; i < navi->sn; i++) {
		if (fabs(navi->stops[i].x() - point.x()) < 3 &&
			fabs(navi->stops[i].y() - point.y() < 3)) {
			ViewTip tip(QString::fromLocal8Bit("��") + QString::number(i + 1, 10, 0) +
				QString::fromLocal8Bit("������վ��"));
			tip.startTimer(500);	//������ͣ��ʾʱ��Ϊ0.5�룬ʱ�䵽�Զ��ر�tip
			QPointF p = qgv->mapToGlobal(QPoint(point.x(), point.y()));
			tip.move(QPoint(p.x(), p.y()));
			tip.exec();			 //��ʾվ����ʾ��Ϣ
		}
	}
	for (int i = 0; i < navi->on; i++) { //��ʾ��֯��Ϣ
		if (fabs(navi->orgs[i].x() - point.x()) < 3 &&
			fabs(navi->orgs[i].y() - point.y() < 3)) {
			ViewTip tip(navi->orgs[i].loc());
			tip.startTimer(500);
			QPointF p = qgv->mapToGlobal(QPoint(point.x(), point.y()));
			tip.move(QPoint(p.x(), p.y()));
			tip.exec();
		}

	}
	QGraphicsScene::mouseMoveEvent(event); //�ص���������¼�
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (qgv == Q_NULLPTR || route_flag == true) { //δ���ص�ͼ����ʾ��ѯ���ʱ������Ӧ��갴���¼�
		QGraphicsScene::mouseMoveEvent(event);
		return;
	}
	QPointF point = event->scenePos();  //��ȡ�������	
	QList<QGraphicsItem*> listItem = items();
	int flag = 0;
	if (event->button() == Qt::LeftButton) { flag = 1, depart_flag = true; }  //�������Ƿ���
	if (event->button() == Qt::RightButton) { flag = 2; arrive_flag = true; } //�������Ƿ���
	for (int i = listItem.size() - 1; i >= 0; i--) {
		MyItem* item = (MyItem*)listItem[i];
		if (item->f() == flag) {
			listItem.removeAt(i);
			delete item;
			break;
		}
	}
	addItem(new MyItem(point.x(), point.y(), flag));
	QGraphicsScene::mousePressEvent(event);  //�ص���������¼�
}

MyItem::MyItem(int x, int y, int f)
	:QGraphicsRectItem(x - 3, y - 3, 7, 7), cx(x), cy(y), cf(f), cs(0) {
	//�������������������������Ҽ���������յ����ò�ͬ������ɫ
	QBrush brush(f == 1 ? QColor(210, 105, 45) : QColor(255, 170, 0));
	QPen pens(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);  //����
	QGraphicsRectItem* item = this;
	item->setPen(pens);
	item->setBrush(brush);
	checkAllStops();  //��������վ�㣬�ҳ������վ�����bs��
}

int& MyItem::x() { return cx; }
int& MyItem::y() { return cy; }
int& MyItem::f() { return cf; }
int& MyItem::s() { return cs; }

int& MyItem::operator[](int x) {
	if (x < 0 || x >= cs) throw "Subscript bound error!";
	return bs[x];	//�����Ѽ��ı��Ϊx��վ����
}

int MyItem::operator()(int x, int y) {	//�����ڼ���������Զ����������
	return (x - cx) * (x - cx) + (y - cy) * (y - cy);
}

MyItem& MyItem::operator<<(int s) {
	if (s < 0 || s >= navi->sn)	return *this;
	int t, dis = (*this)(navi->stops[s].x(), navi->stops[s].y());  //disΪ��ǰͼԪ�㵽վ��s�ľ���
	if (cs == 0 || dis < (t = (*this)(navi->stops[bs[0]].x(), navi->stops[bs[0]].y()))) {
		//��bsû��Ԫ�ؼ�cs==0�������վ��s����
		bs.insert(bs.begin(), s);	//��ͷ����
		cs = 1;
		while (bs.size() > 1)	bs.pop_back();  //ֻ����һ��վ��
		return *this;
	}
	if (dis == t) { bs.push_back(s); cs++; } //���Ѽ��վ��ȣ������Ϊs��վ��ľ�����ͬʱ
	return *this;
}

int MyItem::checkAllStops() {//�������վ�㣬�ҳ������վ�����bs��
	for (int i = 0; i < navi->sn; i++)
		(*this) << navi->stops[i].num();
	return cs; //���ؾ����������ͬ��վ�����
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	setSelected(true);	//��ǰͼԪ��ѡ��
	QGraphicsRectItem::mousePressEvent(event);
}