#include "geometry2D.h"
#include <cmath>

static const double eps = 1e-6;
static inline bool eq(double a, double b)
{
    return ((a - b) < eps) || ((b - a) < eps);
}

double Distance(Point p, Line l, int norm = 2)
{
    double a1, a2, b;
    if (l.intercept_x == GEO_INF)
    {
        a1 = 0, a2 = 1;
        b = l.intercept_y;
    }
    else if (l.intercept_y == GEO_INF)
    {
        a1 = 1, a2 = 0;
        b = l.intercept_x;
    }
    else
    {
        a1 = 1 / l.intercept_x;
        a2 = 1 / l.intercept_y;
        b = 1;
    }
    double dis = a1 * p.x + a2 * p.y - b;
    if (norm = -1)
    {
    }
}
double Distance(Point p, Point q)
{
    return (p - q).norm();
}

Point Common_point(Line a, Line b)
{
    if (eq(a.k, b.k) == 1)
        return Point(GEO_INF, GEO_INF);
    double interX, interY;
    if (eq(a.k, GEO_INF))
    {
        interX = a.intercept_x;
        interY = b(interX);
    }
    else if (eq(b.k, GEO_INF))
    {
        interX = b.intercept_x;
        interY = a(interX);
    }
    else
    {
        interX = -(b.intercept_y - a.intercept_y) / (b.k - a.k);
        interY = a(interX);
    }
    return Point(interX, interY);
}