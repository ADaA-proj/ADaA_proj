#include "shape2D.h"
#include <cmath>

inline bool eq(double a,double b)
{
    return ((a-b)<eps)||((b-a)<eps);
}

Point::Point(double x_, double y_) : x(x_), y(y_) {}
double Point::getx()
{
    return x;
}
double Point::gety()
{
    return y;
}

Line::Line(double interceptx_or_k, double intercepty_or_b, bool use_kb = 0)
{
    if (use_kb == 0)
    {
        intercept_x = interceptx_or_k, intercept_y = intercepty_or_b;
        k = -intercept_y / intercept_x;
    }
    else
    {
        k = interceptx_or_k, intercept_y = intercepty_or_b;
        intercept_x = -intercept_y / k;
    }
}
Line::Line(Point a, Point b)
{
    if (eq(a.gety(), b.gety()))
        intercept_x = GEO_INF, intercept_y = a.gety(), k = 0;
    else if (eq(a.getx(), b.getx()))
        intercept_x = a.getx(), intercept_y = GEO_INF, k = GEO_INF;
    else
    {
        intercept_x = a.getx() - a.gety() * (a.getx() - b.getx()) / (a.gety() - b.gety());
        intercept_y = a.gety() - a.getx() * (a.gety() - b.gety()) / (a.getx() - b.getx());
        k = -intercept_y / intercept_x;
    }
}

Polygon::Polygon(std::vector<Point> p_list_)
{
    p_list.assign(p_list_.begin(), p_list_.end());
}

double ConicSection::Perimeter()
{
    if (e >= 1)
        return GEO_INF;
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
        return GEO_INF;
    return PI * a * b;
}
