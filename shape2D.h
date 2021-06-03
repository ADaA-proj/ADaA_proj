#include <vector>
#include <cstdio>
#include <cmath>

#define GEO_INF 1.0 / 0.0
#define PI 3.1415926535898
const double eps=1e-6;

class Point//also vector
{
    double x = 0, y = 0;

public:
    Point(double x_, double y_);
    double getx();
    double gety();
};

class Line
{
public:
    double intercept_x, intercept_y, k;
    Line(double interceptx_or_k, double intercepty_or_b, bool use_kb);//这里的默认参数和实现的时候的默认参数貌似不能同时使用
    Line(Point a, Point b);
    double operator ()(const double &x)
    {
        return k*x+intercept_y;
    }
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
    Polygon(std::vector<Point> &p_list_);
    Polygon(std::vector<Line>  &l_list_);
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
    double Perimeter();
    double Area();
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
