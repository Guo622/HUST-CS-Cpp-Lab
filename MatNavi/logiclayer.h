#pragma once
#ifndef LOGICLAYER_H_
#define LOGICLAYER_H_

#include <vector>
#include <string>
#include <QString>
using std::vector;

class Stop {   //描述一个站点
private:
	int N, X, Y;   //公交车站点的编号、坐标
public:
	Stop(int n = 0, int x_ = 0, int y_ = 0);
	~Stop() = default;
	int& num();   //返回N
	int& x();	  //返回X
	int& y();	  //返回Y
};

class Line {	//描述一条线路
private:
	int num = 0, size = 0;			//线路的编号、站点数
	vector<int> stops;				//存放站点标号
public:
	Line() = default;
	Line(int n, const vector<int>& st);
	Line(const Line& l);
	Line(Line&& l)noexcept;
	Line& operator=(const Line& l);
	Line& operator=(Line&& l)noexcept;
	~Line() = default;
	int has(int s)const;				//线路是否包含站点s,返回其序号，没有则返回-1	
	int& operator[](int x);				//取线路序号为x的站点
	int s()const;						//返回站点数
};

class Organization {	//描述一个组织
private:
	QString locname;	//组织名		
	int X, Y;			//坐标
public:
	Organization(QString& loc, int x_, int y_);
	Organization() = default;
	~Organization() = default;
	const QString& loc();		//返回组织名
	int& x();		//返回X	
	int& y();		//返回Y
};

class Change {       //转乘类
private:
	int f, t, s;	 //从f路线经s站点转到t路线
public:
	Change(int F, int T, int S);
	int& from();    //返回f
	int& to();		//返回t
	int& stop();	//返回s
	bool operator==(const Change& c)const;    //判断两个转乘是否相等
	bool operator!=(const Change& c)const;
};

class Route {	//一个转乘路径
private:
	vector<Change> changes;	 //转乘路径上的所有转乘
	int n = 0;				 //转乘次数
public:
	Route() = default;
	Route(const Route& r);
	Route(Route&& r)noexcept;
	Route(const vector<Change>& ch);
	Route(const vector<Change>& ch, int nn);  //取ch的前nn个
	Route& operator=(const Route& r);
	Route& operator=(Route&& r)noexcept;
	Route operator+(Route& r);
	Route operator~();						//去重复公交转乘
	operator int& ();						//转乘次数
	Change& operator[](int);				//获取转乘
	bool operator==(const Route& r)const;
	bool operator!=(const Route& r)const;
	int times();							//转乘次数
};

class Node {			//闭包矩阵结点元素，记录转乘方案数和转乘路径
private:
	vector<Route> routes;	//转乘路径
	int n = 0;				//转乘方案数
public:
	Node() = default;
	Node(const Node& n);
	Node(Node&& n)noexcept;
	Node(const vector<Route>& r);
	Node(const vector<Route>& r, int nn);  //取r的前nn个
	Node& operator=(const Node&);
	Node& operator=(Node&& n)noexcept;
	int num();   //方案数
	Node operator+(Node& n);
	Node operator*(Node& n);
	Node operator~();		//去除转乘路线中的环
	Node& operator+=(Node& n);
	Route& operator[](int x);  //获取一个方案
};

class Mat {  //所有公交转乘元素的闭包矩阵
public:
	using VVN = vector<vector<Node>>;  //闭包矩阵类型
private:
	VVN data;			//闭包矩阵
	int r = 0, c = 0;	//闭包矩阵行数和列数
public:
	Mat() = default;
	Mat(const VVN& m);
	Mat(const Mat& m);
	Mat(Mat&& m)noexcept;
	Mat& operator=(const Mat& m);
	Mat& operator=(Mat&& m)noexcept;
	bool noZero();		//判断有无不可到达站点
	vector<Node>& operator[](int x);
	Mat operator*(Mat& m);  //闭包运算乘法 
	Mat operator+(Mat& m);	//闭包运算加法
	Mat& operator*=(Mat& m);
	Mat& operator+=(Mat& m);
};

class Navi { //存储系统基本信息的类
public:
	vector<Stop> stops;				//所有公交站点
	vector<Line> lines;				//所有公交路线
	vector<Organization>orgs;		//所有组织
	Mat raw, close;		//原始闭包矩阵和，闭包转乘矩阵
	int sn, ln, on;		//站点数，线路数，组织数
	Navi(const char* fs, const char* fl, const char* fo);
	~Navi() = default;
	int minChange(int x1, int y1, int x2, int y2,
		int& f, int& t, int& n, vector<Route>& r);				//最少转乘
	int minChange(int& f, int& t, int& n, vector<Route>& r);	//最少转乘
};

#endif