long long infty = ~(1ll << 63);
#define INFINITY *((double *)(&infty))

class Point
{
    double x = 0, y = 0;

public:
    Point(double x_, double y_) : x(x_), y(y_) {}
};

class Line
{
    double intercept_x, intercept_y;

public:
    Line(double intercept_x_, double intercept_y_)
        : intercept_x(intercept_x_), intercept_y(intercept_y_) {}
    Line(double k, double b) : intercept_y(b)
    {
        if (k == 0)
            intercept_x = INFINITY;
        else
            intercept_x = -b / k;
    }
    Line(Point a, Point b)
    {
    }
};

class Shape
{
public:
    virtual double Rerimeter();
    virtual double Area();
};

class Polygon : public Shape
{
public:
    virtual double Rerimeter();
    virtual double Area();
};

class HalfPlane : public Polygon
{
public:
    double Rerimeter();
    double Area();
};

class Triangle : public Polygon
{
public:
    double Rerimeter();
    double Area();
};

class Rectangle : public Polygon
{
public:
    double Rerimeter();
    double Area();
};

class ConicSection : public Shape
{
protected:
    double a, b, c, e;
    double center_x, center_y;

public:
    virtual double Rerimeter();
    virtual double Area();
};

class Elllipse : public ConicSection
{
public:
    virtual double Rerimeter();
    virtual double Area();
};

class Circle : public Elllipse
{
public:
    double Rerimeter();
    double Area();
};

class Parabola : public ConicSection
{
public:
    double Rerimeter();
    double Area();
};

class Hyperbola : public ConicSection
{
    double p;

public:
    double Rerimeter();
    double Area();
};