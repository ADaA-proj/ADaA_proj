#pragma once

#include <vector>
#include <cstdio>
#include <cmath>
#include "geometry2D.h"

#define GEO_INF 1.0 / 0.0
#define PI 3.1415926535898

class Point //also vector
{
public:
    Point(double x_, double y_);
    Point operator+(Point q);
    Point operator-(Point q);
    Point operator*(double k);
    double operator*(Point q);
    double cross(Point q);
    double norm(int n = 2);

    double x = 0, y = 0;
};

class Line
{
public:
    Line(double interceptx_or_k, double intercepty_or_b, bool use_kb = 0); //这里的默认参数和实现的时候的默认参数貌似不能同时使用
    Line(Point a, Point b);
    double operator()(const double &x);

    double intercept_x, intercept_y, k;
};

class Shape
{
public:
    virtual double Perimeter();
    virtual double Area();
};

class Polygon : public Shape
{
protected:
    std::vector<Point> p_list;

public:
    Polygon(const std::vector<Point> &p_list_);
    Polygon(const std::vector<Line> &l_list_);
    virtual double Perimeter();
    virtual double Area();
    void add_point(Point p);
    void add_line(Line l);
};

class HalfPlane : public Polygon
{
public:
    double Perimeter();
    double Area();
};

class Triangle : public Polygon
{
public:
    Triangle(const std::vector<Point> &p_list_);
    double Perimeter();
    double Area();
    Point centroid();    // 重心
    Point orthocenter(); // 垂心
};

class Rectangle : public Polygon
{
public:
    double Perimeter();
    double Area();
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
