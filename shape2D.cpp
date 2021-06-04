#include "shape2D.h"

static const double eps = 1e-6;
static inline bool eq(double a, double b)
{
    return ((a - b) < eps) || ((b - a) < eps);
}

Point::Point(double x_, double y_) : x(x_), y(y_) {}
Point Point::operator+(Point q)
{
    return Point(x + q.x, y + q.y);
}
Point Point::operator-(Point q)
{
    return Point(x - q.x, y - q.y);
}
Point Point::operator*(double k)
{
    return Point(k * x, k * y);
}
double Point::dot(Point q)
{
    return x * q.x + y * q.y;
}
double Point::cross(Point q)
{
    return x * q.y - y * q.x;
}
double Point::norm(int n)
{
    if (n % 2 == 1)
        return pow(pow(abs(x), n) + pow(abs(y), n), 1.0 / n);
    else
        return pow(pow(x, n) + pow(y, n), 1.0 / n);
}

Line::Line(double interceptx_or_k, double intercepty_or_b, bool use_kb)
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
    if (eq(a.y, b.y))
        intercept_x = GEO_INF, intercept_y = a.y, k = 0;
    else if (eq(a.x, b.x))
        intercept_x = a.x, intercept_y = GEO_INF, k = GEO_INF;
    else
    {
        intercept_x = a.x - a.y * (a.x - b.x) / (a.y - b.y);
        intercept_y = a.y - a.x * (a.y - b.y) / (a.x - b.x);
        k = -intercept_y / intercept_x;
    }
}
double Line::operator()(const double &x)
{
    return k * x + intercept_y;
}

Polygon::Polygon(const std::vector<Point> &p_list_) // const allows Rvalue references
{
    p_list.assign(p_list_.begin(), p_list_.end());
}
Polygon::Polygon(const std::vector<Line> &l_list_) // 暂时假设他的传输都是按照一定顺序,后面可以通过凸包、半平面交等方法不要求顺序
{
    for (int i = 1; i < l_list_.size(); ++i)
    {
        p_list.push_back(Common_point(l_list_[i - 1], l_list_[i]));
    }
    p_list.push_back(Common_point(l_list_[l_list_.size() - 1], l_list_[0]));
}
double Polygon::Area()
{
    double ans = 0;
    if (p_list.size() <= 1)
    {
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    for (int i = 1; i < p_list.size(); ++i)
        ans += p_list[i].cross(p_list[i - 1]);
    ans += p_list[0].cross(p_list[p_list.size() - 1]);
    return abs(ans);
}
double Polygon::Perimeter()
{
    double ans = 0;
    if (p_list.size() <= 1)
    {
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    for (int i = 1; i < p_list.size(); ++i)
        ans += Distance(p_list[i], p_list[i - 1]);
    ans += Distance(p_list[0], p_list[p_list.size() - 1]);
    return ans;
}

Triangle::Triangle(const std::vector<Point> &p_list_) : Polygon(p_list_)
{
    if (p_list_.size() != 3)
        exit(1); // 错误处理咋办
    // 能不能做得再robust一点
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
