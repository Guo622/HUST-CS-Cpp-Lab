#include "logiclayer.h"
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>
#include <cmath>
#include <QDebug>

/*	Stop类	*/
Stop::Stop(int n, int x_, int y_) :N(n), X(x_), Y(y_) {}
int& Stop::num() { return N; }
int& Stop::x() { return X; }
int& Stop::y() { return Y; }


/*	Line类	*/
Line::Line(int n, const vector<int>& st) : num(n) {
	stops = st;
	size = stops.size();
}

Line::Line(const Line& l) : num(l.num), size(l.size), stops(l.stops) {}
Line::Line(Line&& l)noexcept :num(l.num), size(l.size), stops(std::move(l.stops)) {}

Line& Line::operator=(const Line& l) {
	if (this == &l)	return *this;
	num = l.num; size = l.size; stops = l.stops;
	return *this;
}

Line& Line::operator=(Line&& l)noexcept {
	if (this == &l)	return *this;
	num = l.num; size = l.size;
	stops = std::move(l.stops);
	return *this;
}

int Line::has(int s)const {
	for (int i = 0; i < size; i++)
		if (stops[i] == s)
			return i;
	return -1;
}

int Line::s() const { return size; }

int& Line::operator[](int x) {
	if (x < 0 || x >= size)	throw "Subscript bound error!";  //下标越界
	return stops[x];
}


/*	Organization类	*/
Organization::Organization(QString& loc, int x_, int y_)
	:locname(loc), X(x_), Y(y_) {}

const QString& Organization::loc() { return locname; }

int& Organization::x() { return X; }
int& Organization::y() { return Y; }


/*	 Change类	*/
Change::Change(int F, int T, int S) : f(F), t(T), s(S) {}

int& Change::from() { return f; }
int& Change::to() { return t; }
int& Change::stop() { return s; }

bool Change::operator==(const Change& c)const {
	return t == c.t && f == c.f && s == c.s;
}

bool Change::operator!=(const Change& c)const {
	return !(*this == c);
}


/*	Route类	*/
Route::Route(const Route& r) :changes(r.changes), n(r.n) {}
Route::Route(Route&& r)noexcept :changes(std::move(r.changes)), n(r.n) {}

Route::Route(const vector<Change>& c) {
	changes = c;
	n = changes.size();
}

Route::Route(const vector<Change>& c, int nn) {
	changes.insert(changes.end(), c.begin(), c.begin() + nn);
	n = nn;
}

Route& Route::operator=(const Route& r) {
	if (this == &r)	return *this;
	changes = r.changes; n = r.n;
	return *this;
}

Route& Route::operator=(Route&& r)noexcept {
	if (this == &r)	return *this;
	changes = std::move(r.changes);
	n = r.n;
	return *this;
}

Route Route::operator~() {  //去重复转乘
	int nn = n;
	vector<Change> temp = changes;
	for (int i = 0; i < nn - 1; i++) {
		for (int j = i + 1; j < nn; j++) {
			if (temp[i].from() == temp[j].to()) {   //构成循环
				for (int k = i, h = j + 1; h < nn; h++, k++)
					temp[k] = temp[h];
				nn -= (j - i + 1);
				j = i;
			}
		}
	}
	return Route(temp, nn);
}

Route::operator int& () { return n; }

Change& Route::operator[](int x) {
	if (x<0 || x>n) throw "Subscript bound error!";   //下标越界
	return changes[x];
}

Route Route::operator+(Route& r) {
	if (r.n == 0)	return r;
	if (n == 0)	return *this;
	if (changes[n - 1].to() != r.changes[0].from())
		throw "Route can't be concated";    //不能相加
	vector<Change> temp;
	temp.insert(temp.end(), changes.begin(), changes.end());
	temp.insert(temp.end(), r.changes.begin(), r.changes.end());
	return ~Route(temp);	//返回去重结果
}

bool Route::operator==(const Route& r)const {
	if (n != r.n)	return false;
	for (int i = 0; i < n; i++)
		if (changes[i] != r.changes[i])
			return false;
	return true;
}

bool Route::operator!=(const Route& r)const { return !(*this == r); }

int Route::times() { return n; }


/*	Node类	*/
Node::Node(const Node& node) :routes(node.routes), n(node.n) {}
Node::Node(Node&& node)noexcept :routes(std::move(node.routes)), n(node.n) {}

Node::Node(const vector<Route>& r) {
	routes = r;
	n = routes.size();
}

Node::Node(const vector<Route>& r, int nn) {
	routes.insert(routes.end(), r.begin(), r.begin() + nn);
	n = nn;
}

Node& Node::operator=(const Node& node) {
	if (this == &node)	return *this;
	routes = node.routes; n = node.n;
	return *this;
}

Node& Node::operator=(Node&& node)noexcept {
	if (this == &node)	return *this;
	routes = std::move(node.routes); n = node.n;
	return *this;
}

int Node::num() { return n; }

Node Node::operator~() {  //去环
	if (n == 0)	return *this;
	int nn = n;
	vector<Route> temp = this->routes;
	for (int i = 0; i < nn - 1; i++) {
		for (int j = i + 1; j < nn; j++) {
			if (temp[i] == temp[j]) { //存在环
				for (int k = j; k < nn - 1; k++)
					temp[k] = temp[k + 1];
				nn--;
			}
		}
	}
	return Node(temp, nn);
}

Node Node::operator+(Node& node) {
	if (n == 0)	return node;
	if (node.n == 0)	return *this;
	vector<Route> temp;
	temp.insert(temp.end(), routes.begin(), routes.end());
	temp.insert(temp.end(), node.routes.begin(), node.routes.end());
	return ~Node(temp);
}

Node& Node::operator+=(Node& node) {
	return *this = *this + node;
}

Node Node::operator*(Node& node) {
	if (n == 0)	return *this;
	if (node.n == 0)	return node;
	vector<Route> temp;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < node.n; j++) {
			try { temp.push_back(routes[i] + node.routes[j]); }
			catch (const char*) { continue; }  //不能相加
		}
	}
	return ~Node(temp);  //返回去重结果
}

Route& Node::operator[](int x) {
	if (x < 0 || x >= n)  throw "Subscript bound error!";  //下标越界
	return routes[x];
}


/*	Mat类	*/
Mat::Mat(const VVN& m) :data(m), r(m.size()) {
	if (r)	c = data[0].size();
	else c = 0;
}

Mat::Mat(const Mat& m) : data(m.data), r(m.r), c(m.c) {}
Mat::Mat(Mat&& m)noexcept :data(std::move(m.data)), r(m.r), c(m.c) {}

Mat& Mat::operator=(const Mat& m) {
	if (this == &m)	return *this;
	data = m.data; r = m.r; c = m.c;
	return *this;
}

Mat& Mat::operator=(Mat&& m)noexcept {
	if (this == &m)	return *this;
	data = std::move(m.data); r = m.r; c = m.c;
	return *this;
}

bool Mat::noZero() {
	if (!c || !r)	return false;
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (data[i][j].num() == 0)
				return false;
	return true;
}

vector<Node>& Mat::operator[](int x) { return data[x]; }

Mat Mat::operator*(Mat& m) {
	if (c != m.r)	throw "Matrix can't mutiply!";   //不能相乘
	VVN temp;
	for (int i = 0; i < r; i++) {
		temp.push_back(vector<Node>());
		for (int j = 0; j < m.c; j++) {
			Node node;
			if (i == j) {
				temp[i].push_back(node);
				continue;
			}
			for (int k = 0; k < c; k++)
				node += (data[i][k] * m.data[k][j]);
			temp[i].push_back(node);
		}
	}
	return Mat(temp);
}

Mat Mat::operator+(Mat& m) {
	if (r != m.r || c != m.c)	throw "Matrix can't add!";
	Mat temp(*this);
	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			temp.data[i][j] += m.data[i][j];
	return temp;
}

Mat& Mat::operator*=(Mat& m) { return *this = *this * m; }

Mat& Mat::operator+=(Mat& m) { return *this = *this + m; }


/*	Navi类	*/
Navi::Navi(const char* fs, const char* fl, const char* fo) {
	using std::ifstream;
	ifstream fins(fs, std::ios::in);
	ifstream finl(fl, std::ios::in);
	QFile f(fo);
	if (!fins || !finl || !f.open(QIODevice::ReadOnly))  //打开文件失败
		throw "error";

	//读取站点
	fins >> sn;
	for (int i = 0; i < sn; i++) {
		int x, y;
		fins >> x >> y;
		stops.push_back({ i + 1,x + 500 ,y - 150 });	//调整坐标
	}
	fins.close();

	//读取线路
	finl >> ln;
	vector<int> size;
	for (int i = 0; i < ln; i++) {
		int s;
		finl >> s;
		size.push_back(s);
	}
	for (int i = 0; i < ln; i++) {
		vector<int>temp;
		for (int j = 0; j < size[i]; j++) {
			int x;
			finl >> x;
			temp.push_back(x - 1);
		}
		lines.push_back({ i + 1,temp });
	}
	finl.close();

	//读取组织
	QTextCodec* code = QTextCodec::codecForName("utf8");
	QTextStream fino(&f);
	fino.setCodec(code);
	while (!fino.atEnd()) {
		QString s, t;
		int x, y;
		fino >> s >> x >> t >> y;
		if (s.isEmpty()) { on = orgs.size(); break; }
		orgs.push_back({ s,x,y });
	}
	f.close();

	//获取原始矩阵
	Mat::VVN temp;
	for (int i = 0; i < ln; i++) {
		temp.push_back(vector<Node>());
		for (int j = 0; j < ln; j++) {
			vector<Route> rs;
			if (i == j) {
				rs.push_back(Route(vector<Change>{Change(i, j, -1)}));
				temp[i].push_back(Node(rs));
				continue;
			}
			Line& li = lines[i];
			Line& lj = lines[j];
			for (int k = 0; k < li.s(); k++) {
				int s1 = li[k];
				if (lj.has(s1) != -1) {
					Route r(vector<Change>{ Change(i, j, s1) });
					rs.push_back(r);
				}
			}
			temp[i].push_back(Node(rs));
		}
	}
	Mat t(temp);
	close = raw = t;

	//获得闭包矩阵
	for (int i = 2; i < ln; i++) {
		raw *= t;
		close += raw;
	}
	raw = t;
}

int Navi::minChange(int x1, int y1, int x2, int y2,
	int& f, int& t, int& n, vector<Route>& r) {   //x1y1为起点，x2y2为终点，f为起始站点，t为终止站点，n为方案数，r为转乘路线
	double dist_from, dist_to, dist_temp;
	f = t = n = 0;
	auto dis = [](int xx1, int yy1, int xx2, int yy2)->double {
		return sqrt((xx1 - xx2) * (xx1 - xx2) + (yy1 - yy2) * (yy1 - yy2));
	};  //lambda表达式，获取两点之间的距离
	dist_from = dis(stops[0].x(), stops[0].y(), x1, y1);
	dist_to = dis(stops[0].x(), stops[0].y(), x2, y2);
	for (int i = 1; i < sn; i++) {  //搜索离起点或终点最近的站点
		dist_temp = dis(stops[i].x(), stops[i].y(), x1, y1);
		if (dist_temp < dist_from) { dist_from = dist_temp; f = i; }
		dist_temp = dis(stops[i].x(), stops[i].y(), x2, y2);
		if (dist_temp < dist_to) { dist_to = dist_temp, t = i; }
	}
	if (f == t)		return 0;  //无需转乘
	return minChange(f, t, n, r);
}

int Navi::minChange(int& f, int& t, int& n, vector<Route>& r) {
	vector<int> line_hasf, line_hast;
	for (int i = 0; i < ln; i++) {  //寻找包含起点或终点相关公交路线下标
		if (lines[i].has(f) != -1)  line_hasf.push_back(i);
		if (lines[i].has(t) != -1)  line_hast.push_back(i);
	}
	Node temp;
	int nt, cnt = 0;
	vector<int> valid;
	for (int i = 0; i < line_hasf.size(); i++) {
		for (int j = 0; j < line_hast.size(); j++) {
			temp = close[line_hasf[i]][line_hast[j]];  //得到转乘方案
			nt = temp.num();
			if (nt == 0)	continue;  //方案数为0
			for (int k = 0; k < nt; k++) {
				int ct = temp[k].times();
				if (cnt == 0 || ct < valid[0]) { //转乘次数更少则重新插入
					valid.insert(valid.begin(), ct);
					r.insert(r.begin(), temp[k]);
					cnt = 1;
				}
				if (ct == valid[0]) {  //转乘次数相等则往后插入
					valid.insert(valid.begin() + cnt, ct);
					r.insert(r.begin() + cnt, temp[k]);
					cnt++;
				}
			}
		}
	}
	while (r.size() > cnt)  r.pop_back();  //去除冗余的路线
	n = valid[0];	//转乘次数
	return cnt;		//方案数
}
