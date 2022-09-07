#pragma once
#ifndef LOGICLAYER_H_
#define LOGICLAYER_H_

#include <vector>
#include <string>
#include <QString>
using std::vector;

class Stop {   //����һ��վ��
private:
	int N, X, Y;   //������վ��ı�š�����
public:
	Stop(int n = 0, int x_ = 0, int y_ = 0);
	~Stop() = default;
	int& num();   //����N
	int& x();	  //����X
	int& y();	  //����Y
};

class Line {	//����һ����·
private:
	int num = 0, size = 0;			//��·�ı�š�վ����
	vector<int> stops;				//���վ����
public:
	Line() = default;
	Line(int n, const vector<int>& st);
	Line(const Line& l);
	Line(Line&& l)noexcept;
	Line& operator=(const Line& l);
	Line& operator=(Line&& l)noexcept;
	~Line() = default;
	int has(int s)const;				//��·�Ƿ����վ��s,��������ţ�û���򷵻�-1	
	int& operator[](int x);				//ȡ��·���Ϊx��վ��
	int s()const;						//����վ����
};

class Organization {	//����һ����֯
private:
	QString locname;	//��֯��		
	int X, Y;			//����
public:
	Organization(QString& loc, int x_, int y_);
	Organization() = default;
	~Organization() = default;
	const QString& loc();		//������֯��
	int& x();		//����X	
	int& y();		//����Y
};

class Change {       //ת����
private:
	int f, t, s;	 //��f·�߾�sվ��ת��t·��
public:
	Change(int F, int T, int S);
	int& from();    //����f
	int& to();		//����t
	int& stop();	//����s
	bool operator==(const Change& c)const;    //�ж�����ת���Ƿ����
	bool operator!=(const Change& c)const;
};

class Route {	//һ��ת��·��
private:
	vector<Change> changes;	 //ת��·���ϵ�����ת��
	int n = 0;				 //ת�˴���
public:
	Route() = default;
	Route(const Route& r);
	Route(Route&& r)noexcept;
	Route(const vector<Change>& ch);
	Route(const vector<Change>& ch, int nn);  //ȡch��ǰnn��
	Route& operator=(const Route& r);
	Route& operator=(Route&& r)noexcept;
	Route operator+(Route& r);
	Route operator~();						//ȥ�ظ�����ת��
	operator int& ();						//ת�˴���
	Change& operator[](int);				//��ȡת��
	bool operator==(const Route& r)const;
	bool operator!=(const Route& r)const;
	int times();							//ת�˴���
};

class Node {			//�հ�������Ԫ�أ���¼ת�˷�������ת��·��
private:
	vector<Route> routes;	//ת��·��
	int n = 0;				//ת�˷�����
public:
	Node() = default;
	Node(const Node& n);
	Node(Node&& n)noexcept;
	Node(const vector<Route>& r);
	Node(const vector<Route>& r, int nn);  //ȡr��ǰnn��
	Node& operator=(const Node&);
	Node& operator=(Node&& n)noexcept;
	int num();   //������
	Node operator+(Node& n);
	Node operator*(Node& n);
	Node operator~();		//ȥ��ת��·���еĻ�
	Node& operator+=(Node& n);
	Route& operator[](int x);  //��ȡһ������
};

class Mat {  //���й���ת��Ԫ�صıհ�����
public:
	using VVN = vector<vector<Node>>;  //�հ���������
private:
	VVN data;			//�հ�����
	int r = 0, c = 0;	//�հ���������������
public:
	Mat() = default;
	Mat(const VVN& m);
	Mat(const Mat& m);
	Mat(Mat&& m)noexcept;
	Mat& operator=(const Mat& m);
	Mat& operator=(Mat&& m)noexcept;
	bool noZero();		//�ж����޲��ɵ���վ��
	vector<Node>& operator[](int x);
	Mat operator*(Mat& m);  //�հ�����˷� 
	Mat operator+(Mat& m);	//�հ�����ӷ�
	Mat& operator*=(Mat& m);
	Mat& operator+=(Mat& m);
};

class Navi { //�洢ϵͳ������Ϣ����
public:
	vector<Stop> stops;				//���й���վ��
	vector<Line> lines;				//���й���·��
	vector<Organization>orgs;		//������֯
	Mat raw, close;		//ԭʼ�հ�����ͣ��հ�ת�˾���
	int sn, ln, on;		//վ��������·������֯��
	Navi(const char* fs, const char* fl, const char* fo);
	~Navi() = default;
	int minChange(int x1, int y1, int x2, int y2,
		int& f, int& t, int& n, vector<Route>& r);				//����ת��
	int minChange(int& f, int& t, int& n, vector<Route>& r);	//����ת��
};

#endif