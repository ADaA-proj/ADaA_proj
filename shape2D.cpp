#include "shape2D.h"
#include "geometry2D.h"
#include <algorithm>
#include <iostream>

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
Point Point::operator+(Point q) const
{
    return Point(x + q.x, y + q.y);
}
Point Point::operator-(Point q) const
{
    return Point(x - q.x, y - q.y);
}
Point Point::operator*(double k) const
{
    return Point(k * x, k * y);
}
Point operator*(double k, Point p)
{
    return p * k;
}
Point Point::operator/(double k) const
{
    return Point(x / k, y / k);
}
double Point::operator*(Point q) const
{
    return x * q.x + y * q.y;
}
double Point::norm(int n) const
{
    if (n % 2 == 1)
        return pow(pow(abs(x), n) + pow(abs(y), n), 1.0 / n);
    else
        return pow(pow(x, n) + pow(y, n), 1.0 / n);
}
Point &Point::normalization()
{
    double l = norm();
    x /= l, y /= l;
    return *this;
}

Line::Line()
{
    tangential = Vec(1, 1).normalization();
    anch = Point(0, 0);
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
    if (k == 0)
    {
        anch = Point(0, intercept_y);
        tangential = Vec(1, 0);
    }
    else if (k == GEO_INF)
    {
        anch = Point(intercept_x, 0);
        tangential = Vec(0, 1);
    }
    else
    {
        anch = Point(0, intercept_y);
        tangential = Vec(1, k).normalization();
    }
}
Line::Line(Point a, Point b) : anch(a)
{
    if (eq(a.y, b.y))
    {
        intercept_x = GEO_INF, intercept_y = a.y, k = 0;
        tangential.x = 1, tangential.y = 0;
    }
    else if (eq(a.x, b.x))
    {
        intercept_x = a.x, intercept_y = GEO_INF, k = GEO_INF;
        tangential.x = 0, tangential.y = 1;
    }
    else
    {
        intercept_x = a.x - a.y * (a.x - b.x) / (a.y - b.y);
        intercept_y = a.y - a.x * (a.y - b.y) / (a.x - b.x);
        k = -intercept_y / intercept_x;
        tangential = Vec(1, k).normalization();
    }
}
Line::operator Vec() const
{
    return tangential;
}
double Line::operator()(const double &x) const
{
    return k * x + intercept_y;
}
double Line::operator[](const double &y) const
{
    if (k == GEO_INF)
        return intercept_x;
    return (y - intercept_y) / k;
}
bool Line::is_on(Point p) const
{
    return p.y == operator()(p.x);
}
double Line::get_k() const
{
    return k;
}
Point Line::get_intercept() const
{
    return Point(intercept_x, intercept_y);
}
Point Line::get_aPoint() const
{
    return anch;
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
Point LineSegment::left_endpoint() const
{
    return Point(min_x, operator()(min_x));
}
Point LineSegment::right_endpoint() const
{
    return Point(max_x, operator()(max_x));
}
Point LineSegment::upper_endpoint() const
{
    return Point(operator[](max_y), max_y);
}
Point LineSegment::lower_endpoint() const
{
    return Point(operator[](min_y), min_y);
}

HalfPlane::HalfPlane(const Line &l, Point p) : Line(l)
{
    if (is_on(p))
        throw "Failed to Create HalfPlane";
    normal = p - anch;
    normal = normal - (normal * tangential) * tangential;
    normal.normalization();
    if (cross(normal, tangential) < 0)
        tangential.x = -tangential.x, tangential.y = -tangential.y;
}
HalfPlane::HalfPlane(const Line &l, int direction) : Line(l)
{
    Point p;
    switch (direction)
    {
    case LEFT:
        p = anch + Point(-1, 0);
        break;
    case RIGHT:
        p = anch + Point(1, 0);
        break;
    case ABOVE:
        p = anch + Point(0, 1);
        break;
    case BELOW:
        p = anch + Point(0, -1);
        break;
    default:
        throw "Wrong Parameter";
        break;
    }
    if (is_on(p))
        throw "Failed to Create HalfPlane";
    normal = p - anch;
    normal = normal - (normal * tangential) * tangential;
    normal.normalization();
}
bool HalfPlane::is_in(Point p) const
{
    return normal * (p - anch) >= 0;
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
double Polygon::area() const
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
double Polygon::perimeter() const
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
bool Polygon::in_Poly(Point a) const //用atan2实现？用向量乘法实现？（暂定前者）
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
Point Polygon::operator()(int k) const
{
    return p_list[k];
}
size_t Polygon::size() const
{
    return p_list.size();
}
double Polygon::MinDis(Point p) const
{
    if (in_Poly(p))
        return 0.0;
    double ans = GEO_INF;
    for (size_t i = 1; i < p_list.size(); ++i)
        ans = std::min(ans, Distance(p, LineSegment(p_list[i - 1], p_list[i])));
    ans = std::min(ans, Distance(p, LineSegment(p_list[0], p_list[p_list.size() - 1])));
    return ans;
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
double Triangle::area() const
{
    return abs(cross(p_list[1] - p_list[0], p_list[2] - p_list[0]));
}
Point Triangle::centroid() const
{
    Point a = p_list[0], b = p_list[1], c = p_list[2];
    return Point((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);
}

ConicSection::ConicSection(double a_, double b_, double e_) : a(a_), b(b_), e(e_), c(e_ * a_) {}
ConicSection::ConicSection(Point center_, Vec long_axis_, Vec short_axis_, double e_) : center(center_), e(e_)
{
    a = long_axis_.norm(), b = short_axis_.norm();
    long_axis = long_axis_.normalization();
    short_axis = short_axis_.normalization();
    c = a * e;
}
double ConicSection::perimeter() const
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
double ConicSection::area() const
{
    if (e >= 1)
        return GEO_INF;
    return PI * a * b;
}
Point ConicSection::get_center() const
{
    return center;
}
Vec ConicSection::get_long_axis() const
{
    return long_axis;
}
Vec ConicSection::get_short_axis() const
{
    return short_axis;
}
double ConicSection::get_a() const
{
    return a;
}
double ConicSection::get_b() const
{
    return b;
}
double ConicSection::get_e() const
{
    return e;
}

Ellipse::Ellipse(double a_, double b_) : ConicSection(a_, b_, std::sqrt(1 - std::pow(b_ / a_, 2))) {}
Ellipse::Ellipse(Point center_, Vec long_axis_, Vec short_axis_)
{
    center = center_;
    a = long_axis_.norm(), b = short_axis_.norm();
    printf("%f, %f\n", short_axis_.x, short_axis_.y);
    long_axis = long_axis_.normalization();
    short_axis = short_axis_.normalization();
    e = std::sqrt(1 - std::pow(b / a, 2));
    c = a * e;
}
double Ellipse::operator()(Point p) const
{
    p = p - center;
    double x = p * long_axis, y = p * short_axis;
    return pow(x / a, 2) + pow(y / b, 2);
}
std::pair<double, double> Ellipse::operator()(double x_) const
{
    x_ -= center.x;
    double a1 = pow(long_axis.x / a, 2) + pow(long_axis.y / b, 2);
    double delta = a1 - pow(x_ / a / b, 2);
    if (delta < 0)
        return std::make_pair(GEO_INF, GEO_INF);
    double a2 = x_ * long_axis.x * long_axis.y * (1 / b / b - 1 / a / a);
    delta = sqrt(delta);
    return std::make_pair((a2 + delta) / a1 + center.y, (a2 - delta) / a1 + center.y);
}
std::pair<double, double> Ellipse::operator[](double y_) const
{
    y_ -= center.y;
    double a1 = pow(long_axis.y / a, 2) + pow(long_axis.x / b, 2);
    double delta = a1 - pow(y_ / a / b, 2);
    if (delta < 0)
        return std::make_pair(GEO_INF, GEO_INF);
    double a2 = y_ * long_axis.x * long_axis.y * (1 / b / b - 1 / a / a);
    delta = sqrt(delta);
    return std::make_pair((a2 + delta) / a1 + center.x, (a2 - delta) / a1 + center.x);
}

Circle::Circle(double r_)
{
    a = b = r_;
}
Circle::Circle(Point center_, double r_)
{
    center = center_;
    a = b = r_;
}
