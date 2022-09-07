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


Navi* navi = NULL;			//地图信息
bool route_flag = false;	//是否查询路径
bool depart_flag = false;	//是否选取步行起点
bool arrive_flag = false;	//是否选取步行终点

MatNavi::MatNavi(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	fl = Q_NULLPTR;
	src = Q_NULLPTR;
	gItem = Q_NULLPTR;
	myTimer = new QTimer(this);
	myTimer->setSingleShot(this);    //定时器只执行一次
	connect(ui.action_minChange, SIGNAL(triggered(bool)), this, SLOT(minChange()));
	connect(ui.action_open, SIGNAL(triggered(bool)), this, SLOT(loadmap()));
	connect(ui.action_exit, SIGNAL(triggered(bool)), this, SLOT(closewnd()));
	connect(ui.action_searchLoc, SIGNAL(triggered(bool)), this, SLOT(seachLoc()));
	connect(myTimer, &QTimer::timeout, this, [=]() {
		QList<QGraphicsItem*>listItem = ui.graphicsView->scene()->items();
		deleteItems();			//查询结果显示时间一到，就删除场景的所有图元
		route_flag = false;		//查询结果显示完毕，可以重新选取步行起点或终点
		});
}

MatNavi::~MatNavi() {
	if (fl) {
		fl->hide();	delete fl; fl = Q_NULLPTR;
		delete myTimer;	deleteItems(); delete navi;
	}
}

void MatNavi::closewnd() {
	if (ui.action_exit->isChecked() == false)	return;	//鼠标点击一次触发两次，第二次触发直接返回
	ui.action_exit->setChecked(false);	//鼠标第一次触发设置状态为false,防止第2次触发进入
	if (fl) { fl->hide(); delete fl; fl = Q_NULLPTR; }
	close();
}

void MatNavi::closeEvent(QCloseEvent* event) {
	if (fl) { fl->hide(); delete fl; fl = Q_NULLPTR; }
	if (src) { src->hide(); delete src, src = Q_NULLPTR; }
}

void MatNavi::loadmap() {	//加载地图
	if (ui.action_open->isChecked() == false)	return;	//鼠标点击触发两次，第二次触发直接返回
	ui.action_open->setChecked(false);	//鼠标第一次触发设置状态为false,防止第2次触发进入
	if (fl) { fl->show(); return; }		//如果先前打开过站点及线路输入窗口则直接显示该窗口
	arrive_flag = depart_flag = false;	//此时未选取步行起点或终点
	fl = new FileLoad();		//如果以前没有打开过站点及线路输入窗口
	fl->setWindowFlags(Qt::WindowStaysOnTopHint);	//设置最外层显示
	fl->myShow(ui.graphicsView);
}

void MatNavi::seachLoc() {		//搜索地点
	if (fl == Q_NULLPTR)	return;
	if (ui.action_searchLoc->isChecked() == false)	return;
	ui.action_searchLoc->setChecked(false);
	if (src) { src->show(); return; }
	src = new SearchLoc(this);
	src->setWindowFlags(Qt::WindowStaysOnTopHint);
	src->show();
}

void MatNavi::deleteItems() { //删除场景的所有图元
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
	//先计算最少转乘的路径,先获得起点坐标和终点坐标
	if (ui.action_minChange->isChecked() == false)	return;	//鼠标点击一次触发两次，第二次触发直接返回
	ui.action_minChange->setChecked(false); //鼠标第一次触发设置状态为false,防止第2次触发进入
	QList<QGraphicsItem*> listItem;
	if (!(depart_flag && arrive_flag))	return; //若没有选中步行起点和终点，则返回
	listItem = ui.graphicsView->scene()->items();
	MyItem* Depart = (MyItem*)listItem[0];
	MyItem* Arrive = (MyItem*)listItem[1];
	if (Depart->f() != 1) {			//若不是步行起点，则交换
		Depart = (MyItem*)listItem[1];
		Arrive = (MyItem*)listItem[0];
	}
	//开始组建图元组：用于显示转乘方案的路径
	QGraphicsEllipseItem* EllipseIte;
	QGraphicsLineItem* LineIte;
	MyScene* scene = (MyScene*)(ui.graphicsView->scene());
	QBrush brush(Qt::green);		//设置颜色
	QPen pens(brush, 6, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);//站点的笔
	QPen penl(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);//线路的笔
	route_flag = true;			//进入查询路径显示期间，不响应步行起点与终点选取
	int cnt, n = 0;				//cnt为可行转乘方案数，n为最少转乘次数
	vector<Route> r;			//返回可行转乘方案
	for (int i = 0; i < Depart->s(); i++) {//接近起点的所有公交站点
		int s = (*Depart)[i];			//获得起点站点编号s
		for (int j = 0; j < Arrive->s(); j++) {//接近终点的所有公交站点
			int t = (*Arrive)[j];			   //获得终点站点编号t
			if (s == t) {					   //起点站和终点站相同不用转乘
				deleteItems();				   //删除先前的转乘方案路径显示
				gItem = new QGraphicsItemGroup();
				EllipseIte = new QGraphicsEllipseItem(Depart->x(), Depart->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(brush);
				gItem->addToGroup(EllipseIte);   //步行起点的位置
				LineIte = new QGraphicsLineItem(Depart->x(), Depart->y(), Arrive->x(), Arrive->y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);		 //到步行终点的路径
				EllipseIte = new QGraphicsEllipseItem(Arrive->x(), Arrive->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(QBrush(QColor(255, 170, 0)));
				gItem->addToGroup(EllipseIte);	 //步行终点的位置
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
				continue;
			}
			cnt = navi->minChange(s, t, n, r);	//得到的转乘方案数
			for (int k = 0; k < cnt; k++) {		 //对于第K个转乘方案即route=r[K], 转乘次数都为n 
				deleteItems();
				gItem = new QGraphicsItemGroup();
				EllipseIte = new QGraphicsEllipseItem(Depart->x(), Depart->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(brush);
				gItem->addToGroup(EllipseIte);	 //步行起点的位置
				int fr = s, to = t;				 //起始站fr与终点站to
				int fm, tm;						 //已乘线路fm与转乘线路tm
				int begin, end;					 //线路中的起始站点序号begin,终止站点序号end
				LineIte = new QGraphicsLineItem(Depart->x(), Depart->y(),
					navi->stops[s].x(), navi->stops[s].y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);				 //到起点站	
				if (n == 1 && r[k][0].stop() == -1) {	 //即从i路到i路(此时S()==-1)不用转乘
					fm = r[k][0].from();				 //已乘线路序号fm
					begin = navi->lines[fm].has(fr);	 //起始站点在线路中的序号
					end = navi->lines[fm].has(to);		 //终止站点在线路中的序号
					if (begin > end) { tm = begin; begin = end; end = tm; }
					for (int x = begin; x < end; x++) {	  //从起始站点下一序号到终止站点序号
						fr = navi->lines[fm][x];		  //得到该站点序号对应的站点编号
						to = navi->lines[fm][x + 1];	  //得到该站点序号对应的站点编号
						LineIte = new QGraphicsLineItem(navi->stops[fr].x(), navi->stops[fr].y(),
							navi->stops[to].x(), navi->stops[to].y());
						LineIte->setPen(penl);
						gItem->addToGroup(LineIte);       //到下一站的路径
					}
				}
				else {
					for (int x = 0; x < n; x++) {		 //对于每个转乘
						fm = r[k][x].from();			 //对于每个转乘的起始线路
						begin = navi->lines[fm].has(fr);
						to = r[k][x].stop();			 //对于起始线路的转乘站点
						end = navi->lines[fm].has(to);
						if (begin > end) { tm = begin; begin = end; end = tm; }
						for (int y = begin; y < end; y++) {		//从起始站点下一序号到终止站点序号
							int ff = navi->lines[fm][y];		//得到该站点序号对应的站点编号
							int tt = navi->lines[fm][y + 1];	//得到该站点序号对应的站点编号
							LineIte = new QGraphicsLineItem(navi->stops[ff].x(), navi->stops[ff].y(),
								navi->stops[tt].x(), navi->stops[tt].y());
							LineIte->setPen(penl);
							gItem->addToGroup(LineIte);			//到下一站的路径
						}
						fr = to;		//作为下一起点
					}
					fm = r[k][n - 1].to();		//对于最后乘坐的线路
					begin = navi->lines[fm].has(fr);
					end = navi->lines[fm].has(t);
					if (begin > end) { tm = begin; begin = end; end = tm; }
					for (int x = begin; x < end; x++) {		 //从起始站点下一序号到终止站点序号
						int ff = navi->lines[fm][x];		//得到该站点序号对应的站点编号
						int tt = navi->lines[fm][x + 1];	//得到该站点序号对应的站点编号
						LineIte = new QGraphicsLineItem(navi->stops[ff].x(), navi->stops[ff].y(),
							navi->stops[tt].x(), navi->stops[tt].y());
						LineIte->setPen(penl);
						gItem->addToGroup(LineIte);			 //到下一站的路径
					}
				}
				LineIte = new QGraphicsLineItem(navi->stops[t].x(), navi->stops[t].y(), Arrive->x(), Arrive->y());
				LineIte->setPen(penl);
				gItem->addToGroup(LineIte);				 //到步行终点的路径
				EllipseIte = new QGraphicsEllipseItem(Arrive->x(), Arrive->y(), 6, 6);
				EllipseIte->setPen(pens);
				EllipseIte->setBrush(QBrush(QColor(255, 170, 0)));
				gItem->addToGroup(EllipseIte);			 //步行终点的位置
				scene->addItem(gItem);
				ui.graphicsView->setScene(scene);
			}
		}
	}
	this->myTimer->start(5000);			 //展示查询的路径结果5秒
}
void MatNavi::showPath() {   //显示线路
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
	qgv = Q_NULLPTR; //没有加载地图文件时
}

void MyScene::stopLines(QGraphicsView* parent) { //加载地图站点和线路
	QSize viewsize = parent->size();	//取得graphicsview视图区域大小
	MyScene* scene;
	if (parent->scene() != Q_NULLPTR)	delete parent->scene();
	scene = new MyScene(parent);		//创建scene
	scene->setSceneRect(0, 0, viewsize.width(), viewsize.height());
	scene->qgv = parent;
	QBrush brush(Qt::red);   //设置颜色
	QPen pens(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);  //站点的笔

	//显示所有站点
	for (int i = 0; i < navi->sn; i++)
		scene->addEllipse(navi->stops[i].x(), navi->stops[i].y(), 6, 6, pens, brush);
	brush.setColor(Qt::blue);

	//显示所有线路
	QPen penl(brush, 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);  //线路的笔
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

	//显示所有组织
	QPen peno(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin); //组织的笔
	for (int i = 0; i < navi->on; i++)
		scene->addEllipse(navi->orgs[i].x(), navi->orgs[i].y(), 6, 6, peno, brush);
	parent->setScene(scene);
	parent->show();
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	if (qgv == Q_NULLPTR) { //如果没有加载地图文件
		QGraphicsScene::mouseMoveEvent(event);
		return;
	}
	QPointF point = event->scenePos();	//获取鼠标移动时的坐标	
	for (int i = 0; i < navi->sn; i++) {
		if (fabs(navi->stops[i].x() - point.x()) < 3 &&
			fabs(navi->stops[i].y() - point.y() < 3)) {
			ViewTip tip(QString::fromLocal8Bit("第") + QString::number(i + 1, 10, 0) +
				QString::fromLocal8Bit("个公交站点"));
			tip.startTimer(500);	//设置悬停显示时间为0.5秒，时间到自动关闭tip
			QPointF p = qgv->mapToGlobal(QPoint(point.x(), point.y()));
			tip.move(QPoint(p.x(), p.y()));
			tip.exec();			 //显示站点提示信息
		}
	}
	for (int i = 0; i < navi->on; i++) { //显示组织信息
		if (fabs(navi->orgs[i].x() - point.x()) < 3 &&
			fabs(navi->orgs[i].y() - point.y() < 3)) {
			ViewTip tip(navi->orgs[i].loc());
			tip.startTimer(500);
			QPointF p = qgv->mapToGlobal(QPoint(point.x(), point.y()));
			tip.move(QPoint(p.x(), p.y()));
			tip.exec();
		}

	}
	QGraphicsScene::mouseMoveEvent(event); //回调基类鼠标事件
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (qgv == Q_NULLPTR || route_flag == true) { //未加载地图及显示查询结果时，不响应鼠标按下事件
		QGraphicsScene::mouseMoveEvent(event);
		return;
	}
	QPointF point = event->scenePos();  //获取鼠标坐标	
	QList<QGraphicsItem*> listItem = items();
	int flag = 0;
	if (event->button() == Qt::LeftButton) { flag = 1, depart_flag = true; }  //检查左键是否按下
	if (event->button() == Qt::RightButton) { flag = 2; arrive_flag = true; } //检查左键是否按下
	for (int i = listItem.size() - 1; i >= 0; i--) {
		MyItem* item = (MyItem*)listItem[i];
		if (item->f() == flag) {
			listItem.removeAt(i);
			delete item;
			break;
		}
	}
	addItem(new MyItem(point.x(), point.y(), flag));
	QGraphicsScene::mousePressEvent(event);  //回调基类鼠标事件
}

MyItem::MyItem(int x, int y, int f)
	:QGraphicsRectItem(x - 3, y - 3, 7, 7), cx(x), cy(y), cf(f), cs(0) {
	//根据鼠标左键点击步行起点和鼠标右键点击步行终点设置不同画笔颜色
	QBrush brush(f == 1 ? QColor(210, 105, 45) : QColor(255, 170, 0));
	QPen pens(brush, 4, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);  //画笔
	QGraphicsRectItem* item = this;
	item->setPen(pens);
	item->setBrush(brush);
	checkAllStops();  //检索所有站点，找出最近的站点存于bs中
}

int& MyItem::x() { return cx; }
int& MyItem::y() { return cy; }
int& MyItem::f() { return cf; }
int& MyItem::s() { return cs; }

int& MyItem::operator[](int x) {
	if (x < 0 || x >= cs) throw "Subscript bound error!";
	return bs[x];	//返回已检测的编号为x的站点编号
}

int MyItem::operator()(int x, int y) {	//仅用于检测两点距离远近，不开方
	return (x - cx) * (x - cx) + (y - cy) * (y - cy);
}

MyItem& MyItem::operator<<(int s) {
	if (s < 0 || s >= navi->sn)	return *this;
	int t, dis = (*this)(navi->stops[s].x(), navi->stops[s].y());  //dis为当前图元点到站点s的距离
	if (cs == 0 || dis < (t = (*this)(navi->stops[bs[0]].x(), navi->stops[bs[0]].y()))) {
		//若bs没有元素即cs==0，或距离站点s更近
		bs.insert(bs.begin(), s);	//从头插入
		cs = 1;
		while (bs.size() > 1)	bs.pop_back();  //只保留一个站点
		return *this;
	}
	if (dis == t) { bs.push_back(s); cs++; } //和已检测站点比，到标号为s的站点的距离相同时
	return *this;
}

int MyItem::checkAllStops() {//检测所有站点，找出最近的站点存于bs中
	for (int i = 0; i < navi->sn; i++)
		(*this) << navi->stops[i].num();
	return cs; //返回距离最近且相同的站点个数
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	setSelected(true);	//当前图元被选中
	QGraphicsRectItem::mousePressEvent(event);
}