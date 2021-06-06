#include "geometry2D.h"
#include <cmath>
#include <algorithm>
#include <deque>

static const double eps = 1e-6;
static inline bool eq(double a, double b)
{
    return ((a - b) < eps) || ((b - a) < eps);
}
static inline double abs(double x)
{
    return (x < 0) ? -x : x;
}

double cross(Vec p, Vec q)
{
    return p.x * q.y - p.y * q.x;
}

double degree(Point p, Point q)
{
    return atan2(p.y, p.x) - atan2(q.y, q.x);
}

Polygon ConvexHall(std::vector<Point> point_list) // Graham scan O(nlogn)//用传输polygon得引用的方法来避免传输vector？
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
        while (cross(out[top] - out[top - 1], point_list[i] - out[top]) < 0)
        {
            out.pop_back();
            --top;
        }
        out.push_back(point_list[i]);
        ++top;
    }

    return Polygon(out);
}

double Distance(Point p, const Line &l, int norm)
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
    if (norm == 2)
    {
        dis = dis / Point(a1, a2).norm(2);
        return dis;
    }
    else if (norm == -1)
    {
    }
}

double Distance(Point p, Point q)
{
    return (p - q).norm();
}

double Distance(Point p, LineSegment l) //p到ab线段距离
{
    Point a = l.left_endpoint(), b = l.right_endpoint();
    if (abs(degree(b - a, p - a)) <= (PI / 2.0 + eps) && abs(degree(a - b, p - b)) <= PI / 2.0 + eps)
    {
        return Distance(p, Line(a, b));
    }
    else
    {
        return std::min(Distance(a, p), Distance(b, p));
    }
}

Point Common_point(const Line &a, const Line &b)
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

bool checkback(std::deque<HalfPlane> &deq, HalfPlane hp)
{
    std::deque<HalfPlane>::iterator it = deq.end();
    it--;
    HalfPlane back1 = *it;
    it--;
    HalfPlane back2 = *it;
    return cross(Point(hp), Common_point(back1, back2)) < eps;
}

bool checkfront(std::deque<HalfPlane> &deq, HalfPlane hp)
{
    std::deque<HalfPlane>::iterator it = deq.begin();
    it++;
    HalfPlane front1 = *it;
    it++;
    HalfPlane front2 = *it;
    return cross(Point(hp), Common_point(front1, front2)) < eps;
}

Polygon HalfPlaneIntersection(std::vector<HalfPlane> Line_list) //未完成检验
{
    if (Line_list.size() < 3)
    {
        //错误处理？
        //定义一个“空”的宏变量？
    }
    sort(Line_list.begin(), Line_list.end());
    std::deque<HalfPlane> deq;
    for (size_t i = 0; i < Line_list.size() - 1; ++i)
    {
        if (eq(Line_list[i].degree, Line_list[i + 1].degree))
        {
            Line_list.erase((int)i);
            --i;
        }
    }
    for (size_t i = 0; i < Line_list.size(); ++i)
    {
        while (!deq.empty() && checkback(deq, Line_list[i]))
            deq.pop_back();
        while (!deq.empty() && checkfront(deq, Line_list[i]))
            deq.pop_front();
        deq.push_back(Line_list[i]);
    }
    while (!deq.empty() && checkback(deq, deq.front()))
        deq.pop_back();
    while (!deq.empty() && checkfront(deq, deq.back()))
        deq.pop_front();
    if (deq.size() < 3)
    {
        throw "error!";
    }
    return Polygon(deq);
}
