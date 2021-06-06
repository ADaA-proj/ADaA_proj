#include "shape2D.h"
#include "geometry2D.h"

static const double eps = 1e-6;
static inline bool eq(double a, double b)
{
    return ((a - b) < eps) || ((b - a) < eps);
}
static inline double abs(double x)
{
    return (x < 0) ? -x : x;
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
Point operator*(double k, Point p)
{
    return p * k;
}
double Point::operator*(Point q)
{
    return x * q.x + y * q.y;
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
double Line::operator[](const double &y)
{
    if (k == GEO_INF)
        return intercept_x;
    return (y - intercept_y) / k;
}

LineSegment::LineSegment(Point a, Point b) : Line(a, b)
{
    if (a.x > b.x)
        max_x = a.x, min_x = b.x;
    else
        max_x = b.x, min_x = a.x;
    if (a.y > b.y)
        max_y = a.y, min_y = b.y;
    else
        max_y = b.y, min_y = a.y;
}
Point LineSegment::left_endpoint()
{
    return Point(min_x, operator()(min_x));
}
Point LineSegment::right_endpoint()
{
    return Point(max_x, operator()(max_x));
}
Point LineSegment::upper_endpoint()
{
    return Point(operator[](max_y), max_y);
}
Point LineSegment::lower_endpoint()
{
    return Point(operator[](min_y), min_y);
}

Polygon::Polygon(const std::vector<Point> &p_list_) // const allows Rvalue references
{
    set_with_point(p_list_);
}
Polygon::Polygon(const std::vector<Line> &l_list_) // 暂时假设他的传输都是按照一定顺序,后面可以通过凸包、半平面交等方法不要求顺序
{
    size_t len = p_list.size();
    for (size_t i = 1; i < len; ++i)
    {
        p_list.push_back(Common_point(l_list_[i - 1], l_list_[i]));
    }
    p_list.push_back(Common_point(l_list_[l_list_.size() - 1], l_list_[0]));
}
void Polygon::set_with_point(const std::vector<Point> &p_list_) // 所有的构造函数都直接调用这种set_with_xxx得了
{
    if (p_list_.size() < 3)
        throw "Incorrect Number of Points!";
    p_list.assign(p_list_.begin(), p_list_.end());
}
double Polygon::area()
{
    double ans = 0;
    if (p_list.size() <= 1)
    {
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    size_t len = p_list.size();
    for (size_t i = 1; i < len; ++i)
        ans += cross(p_list[i], p_list[i - 1]);
    ans += cross(p_list[0], p_list[p_list.size() - 1]);
    return abs(ans);
}
double Polygon::perimeter()
{
    double ans = 0;
    if (p_list.size() <= 1)
    {
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    size_t len = p_list.size();
    for (size_t i = 1; i < len; ++i)
        ans += Distance(p_list[i], p_list[i - 1]);
    ans += Distance(p_list[0], p_list[p_list.size() - 1]);
    return ans;
}
bool Polygon::in_Poly(Point a) //用atan2实现？用向量乘法实现？（暂定前者）
{
    double ans = 0;
    if (p_list.size() <= 1)
    {
        printf("empty polygon!");
        throw "empty polygon!";
        return 0;
    }
    size_t len = p_list.size();
    for (size_t i = 1; i < len; ++i)
    {
        ans += degree(p_list[i], p_list[i - 1]);
    }
    ans += degree(p_list[0], p_list[p_list.size() - 1]);
    return abs(ans / (2.0 * PI)) > eps;
}

Triangle::Triangle(const std::vector<Point> &p_list_)
{
    set_with_point(p_list_);
}
void Triangle::set_with_point(const std::vector<Point> &p_list_)
{
    if (p_list_.size() != 3)
        throw "Incorrect Number of Points!";
    p_list.assign(p_list_.begin(), p_list_.end());
}
double Triangle::area()
{
    return abs(cross(p_list[1] - p_list[0], p_list[2] - p_list[0]));
}
Point Triangle::centroid()
{
    Point a = p_list[0], b = p_list[1], c = p_list[2];
    return Point((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);
}

double ConicSection::perimeter()
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
double ConicSection::area()
{
    if (e >= 1)
        return GEO_INF;
    return PI * a * b;
}
