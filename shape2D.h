#pragma once

#include <vector>
#include <cstdio>
#include <cmath>

#define GEO_INF 1.0 / 0.0
#define PI 3.1415926535898

class Point //also vector
{
public:
    Point() {}
    Point(double x_, double y_);
    Point operator+(Point q) const;
    Point operator-(Point q) const;
    Point operator*(double k) const;
    friend Point operator*(double k, Point p);
    double operator*(Point q) const;
    double norm(int n = 2) const;
    Point &normalization();

    double x = 0, y = 0;
};
typedef Point Vec;

class Line
{
    friend double Distance(Point p, const Line &l, int norm);
    friend Point Common_point(const Line &a, const Line &b);

public:
    Line();
    Line(double interceptx_or_k, double intercepty_or_b, bool use_kb = 0); //这里的默认参数和实现的时候的默认参数貌似不能同时使用
    Line(Point a, Point b);
    operator Vec() const;
    double operator()(const double &x) const;
    double operator[](const double &y) const;
    bool is_on(Point p) const;

protected:
    Vec tangential;
    Point anch;
    double intercept_x = 0, intercept_y = 0, k = 1;
};

class LineSegment : public Line
{
public:
    LineSegment() {}
    LineSegment(Point a, Point b);
    Point left_endpoint() const;
    Point right_endpoint() const;
    Point upper_endpoint() const;
    Point lower_endpoint() const;

private:
    double min_x = -1, max_x = 1;
    double min_y = -1, max_y = 1;
};

class HalfPlane : public Line
{
#define LEFT 1
#define RIGHT 2
#define ABOVE 3
#define BELOW 4

public:
    HalfPlane() {}
    HalfPlane(const Line &l, int direction);
    HalfPlane(const Line &l, Point p);
    bool is_in(Point p) const;
    //double Perimeter();
    //double Area();

private:
    Vec normal;
};

class Shape
{
};

class Polygon : public Shape
{
protected:
    std::vector<Point> p_list;

public:
    Polygon() {}
    Polygon(const std::vector<Point> &p_list_);
    Polygon(const std::vector<Line> &l_list_);
    void set_with_point(const std::vector<Point> &p_list_);
    double perimeter() const;
    double area() const;
    //void add_point(Point p);
    //void add_line(Line l);
    bool in_Poly(Point a) const;
};

class Triangle : public Polygon
{
public:
    Triangle() {}
    Triangle(const std::vector<Point> &p_list_);
    void set_with_point(const std::vector<Point> &p_list_);
    double area() const;
    Point centroid() const; // 重心
    //Point orthocenter(); // 垂心
};

class Rectangle : public Polygon
{
public:
    Rectangle() {}
    //double area();

private:
    double l, d;
};

class ConicSection : public Shape
{
protected:
    double a, b, c, e;
    double center_x, center_y;

public:
    double perimeter();
    double area();
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
