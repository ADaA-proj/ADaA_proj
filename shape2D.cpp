#include "shape2D.h"
#include <cmath>

Point::Point(double x_, double y_) : x(x_), y(y_) {}
double Point::getx()
{
    return x;
}
double Point::gety()
{
    return y;
}

Line::Line(double intercept_x_, double intercept_y_)
    : intercept_x(intercept_x_), intercept_y(intercept_y_) {}
Line::Line(Point a, Point b)
{
    if (a.gety() == b.gety())
        intercept_x = INFINITY, intercept_y = a.gety();
    else if (a.getx() == b.getx())
        intercept_x = a.getx(), intercept_y = INFINITY;
    else
    {
        intercept_x = a.getx() - a.gety() * (a.getx() - b.getx()) / (a.gety() - b.gety());
        intercept_y = a.gety() - a.getx() * (a.gety() - b.gety()) / (a.getx() - b.getx());
    }
}

double ConicSection::Perimeter()
{
    if (e >= 1)
        return INFINITY;
    else if (e == 0)
        return 2 * PI * a;
    else
    {
        // An approximate formula for the perimeter of an ellipse
        double Q = a + b;
        double h = std::pow((a - b) / (a + b), 2);
        double area = PI * Q * (1 + 3 * h / (10 + std::sqrt(4 - 3 * h)));
        if (b < a / 2)
        {
            double m = 22 / 7 / PI - 1;
            double n = std::pow(1 - b / a, 33.697);
            area *= (1 + n * m);
        }
        return area;
    }
}
double ConicSection::Area()
{
    if (e >= 1)
        return INFINITY;
    return PI * a * b;
}