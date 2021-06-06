#pragma once

#include <vector>
#include <cstdio>
#include <cmath>

#define GEO_INF 1.0 / 0.0
#define PI 3.1415926535898

class Point //also vector
{
public:
    Point(double x_, double y_);
    Point operator+(Point q);
    Point operator-(Point q);
    Point operator*(double k);
    friend Point operator*(double k, Point p);
    double operator*(Point q);
    double norm(int n = 2);

    double x = 0, y = 0;
};
typedef Point Vec;

class Line
{
public:
    Line(double interceptx_or_k, double intercepty_or_b, bool use_kb = 0); //这里的默认参数和实现的时候的默认参数貌似不能同时使用
    Line(Point a, Point b);
    double operator()(const double &x);
    double operator[](const double &y);

    double intercept_x, intercept_y, k;
};

class LineSegment : public Line
{
public:
    LineSegment(Point a, Point b);
    Point left_endpoint();
    Point right_endpoint();
    Point upper_endpoint();
    Point lower_endpoint();

private:
    double min_x, max_x;
    double min_y, max_y;
};

class Shape
{
};

class Polygon : public Shape
{
protected:
    std::vector<Point> p_list;

public:
    Polygon(const std::vector<Point> &p_list_);
    Polygon(const std::vector<Line> &l_list_);
    double Perimeter();
    double Area();
    //void add_point(Point p);
    //void add_line(Line l);
    bool in_Poly(Point a);
};

class HalfPlane : public Vec//public Polygon 先用向量左侧表示要求的半平面
{
public:
    double degree;Point anch;
    HalfPlane(double x_,double y_,Point anch_);
    bool operator<(HalfPlane b);
    operator Line();//找到向量所表示的直线形式，但是该过程当中会损失半平面对应左右侧的关系
    //double Perimeter();
    //double Area();
};

class Triangle : public Polygon
{
public:
    Triangle(const std::vector<Point> &p_list_);
    //double Perimeter();
    //double Area();
    Point centroid(); // 重心
    //Point orthocenter(); // 垂心
};

class Rectangle : public Polygon
{
public:
    //double Perimeter();
    //double Area();
};

class ConicSection : public Shape
{
protected:
    double a, b, c, e;
    double center_x, center_y;

public:
    double Perimeter();
    double Area();
};

class Ellipse : public ConicSection
{
public:
};

class Circle : public Ellipse
{
public:
};

class Parabola : public ConicSection
{
public:
};

class Hyperbola : public ConicSection
{
    double p;

public:
};
