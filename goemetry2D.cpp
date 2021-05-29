#include "geometry2D.h"
#include <cmath>

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
    double dis = a1 * p.getx() + a2 * p.gety() - b;
    if (norm = -1)
    {
    }
}