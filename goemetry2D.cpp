#include "geometry2D.h"
#include <cmath>
#include <algorithm>

static const double eps = 1e-6;
static inline bool eq(double a, double b)
{
    return ((a - b) < eps) || ((b - a) < eps);
}

Polygon ConvexHall(std::vector<Point> point_list) // Graham scan O(nlogn)
{
    size_t len = point_list.size();
    if (len < 3)
        exit(1);

    auto cmp1 = [](Point &a, Point &b)
    { return (a.y == b.y) ? (a.x < b.x) : (a.y < b.y); };

    std::sort(point_list.begin(), point_list.end(), cmp1);
    std::vector<Point> out;
    out.push_back(point_list[0]);

    auto cmp2 = [&point_list](Point &a, Point &b)
    { return (atan2(a.y - point_list[0].y, a.x - point_list[0].x) != atan2(b.y - point_list[0].y, b.x - point_list[0].x))
                 ? (atan2(a.y - point_list[0].y, a.x - point_list[0].x)) < (atan2(b.y - point_list[0].y, b.x - point_list[0].x))
                 : (a.x < b.x); };
    std::sort(point_list.begin() + 1, point_list.end(), cmp2);

    out.push_back(point_list[1]);
    size_t top = 1;
    for (size_t i = 2; i < len; ++i)
    {
        while ((out[top] - out[top - 1]).cross(point_list[i] - out[top]) < 0)
        {
            out.pop_back();
            --top;
        }
        out.push_back(point_list[i]);
        ++top;
    }

    return Polygon(out);
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
    Point tmp = p - q;
    return sqrt(tmp * tmp);
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