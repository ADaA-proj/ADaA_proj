#pragma once

#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iostream>


#define GEO_INF 1.0 / 0.0
#define PI 3.1415926535898
const double Nan=std::sqrt(-1);

class Point //also vector
{
public:
    Point() {}
    Point(double x_, double y_);
    Point operator+(Point q) const;
    Point operator-(Point q) const;
    Point operator*(double k) const;
    friend Point operator*(double k, Point p);
    Point operator/(double k) const;
    double operator*(Point q) const;
    double norm(int n = 2) const;
    Point &normalization();
    void rotation(double theta);
    void translation(Point delta);

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
    double get_k() const;
    Point get_intercept() const;
    Point get_aPoint() const;
    void rotation(double theta);
    void translation(Vec delta);

protected:
    Vec tangential;
    Point anch;
    double intercept_x = 0, intercept_y = 0, k = 1;
    int flag = 0;
};

class Line_with_args : public Line
{
protected:
    std::vector<double*> args;
public:
    Line_with_args(Point a);
    Line_with_args(double k_);
    size_t arg_size()const;
    void set_args(const std::vector<double> &v);
    void set_args(double arg);
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
    friend bool cmp(const HalfPlane &a, const HalfPlane &b);

public:
    HalfPlane() {}
    HalfPlane(const Line &l, int direction);
    HalfPlane(const Line &l, Point p);
    bool is_in(Point p) const;

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
    Point operator()(int k) const;
    size_t size() const;
    double MinDis(Point p) const;
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
    double a = 1, b = 1, c = 0, e = 0;
    Point center = Point(0, 0);
    Vec long_axis = Vec(1, 0), short_axis = Vec(0, 1);

public:
    ConicSection() {}
    ConicSection(double a_, double b_, double e_);
    ConicSection(Point center_, double a_, double b_, double e_, Vec long_axis_);
    double perimeter() const;
    double area() const;
    Point get_center() const;
    Vec get_long_axis() const;
    Vec get_short_axis() const;
    double get_a() const;
    double get_b() const;
    double get_e() const;
    void rotation(double theta);
    void translation(Vec delta);
};

class Ellipse : public ConicSection
{
public:
    Ellipse() {}
    Ellipse(double a_, double b_);
    Ellipse(Point center_, Vec long_axis_, Vec short_axis_);
    double operator()(Point p) const;
    std::pair<double, double> operator()(double x_) const;
    std::pair<Point, Point> operator()(Line l) const;
    std::pair<Point, Point> Line_intersection_for_stardard(Line l) const;
    std::pair<double, double> operator[](double y_) const;
};

class Circle : public Ellipse
{
public:
    Circle() {}
    Circle(double r_);
    Circle(Point center_, double r_);
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
