#include <vector>

long long infty = ~(1ll << 63);
#define INFINITY *((double *)(&infty))
#define PI 3.1415926535898

class Point
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
    double intercept_x, intercept_y;
    Line(double intercept_x_, double intercept_y_);
    Line(Point a, Point b);
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
    Polygon(std::vector<Point> p_list_);
    Polygon(std::vector<Line> l_list_);
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
