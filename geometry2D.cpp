#include "geometry2D.h"
#include <cmath>
#include <algorithm>
#include <deque>

static const double eps = 1e-6;
static inline bool eq(double a, double b)
{
    return ((a - b) < eps) && ((b - a) < eps);
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

Polygon ConvexHall(std::vector<Point> point_list) // Graham scan O(nlogn)
{
    size_t len = point_list.size();
    if (len < 3)
    {
        throw "too few points to form a Polygon!";
    }

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
    if (eq(l.intercept_x, GEO_INF))
    {
        a1 = 0, a2 = 1;
        b = l.intercept_y;
    }
    else if (eq(l.intercept_y, GEO_INF))
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
    a1 = abs(a1), a2 = abs(a2);
    if (norm == -1)
        return dis / (a1 + a2);
    else if (norm == 1)
        return dis / std::max(a1, a2);
    else
    {
        double p = double(norm) / (norm - 1);
        double tmp = pow(pow(a1, p) + pow(a2, p), 1 / p);
        return dis / tmp;
    }
}

double Distance(Point p, Point q)
{
    return (p - q).norm();
}

double Distance(Point p, LineSegment l) //p???ab????????????
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

double Distance(std::pair<Point, Point> p)
{
    return Distance(p.first, p.second);
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

static bool checkback(std::deque<HalfPlane> &deq, HalfPlane hp)
{
    std::deque<HalfPlane>::iterator it = deq.end();
    it--;
    HalfPlane back1 = *it;
    it--;
    HalfPlane back2 = *it;
    return cross(Point(hp), Common_point(back1, back2)) < eps;
}
static bool checkfront(std::deque<HalfPlane> &deq, HalfPlane hp)
{
    std::deque<HalfPlane>::iterator it = deq.begin();
    it++;
    HalfPlane front1 = *it;
    it++;
    HalfPlane front2 = *it;
    return cross(Point(hp), Common_point(front1, front2)) < eps;
}
Polygon HalfPlaneIntersection(std::vector<HalfPlane> Line_list)
{
    if (Line_list.size() < 3)
    {
        throw "too few halfplane for intersection!";
    }
    auto cmp = [](const HalfPlane &a, const HalfPlane &b)
    {
        return (a.get_k() == b.get_k()) ? (a.get_k() > b.get_k()) : (a.is_in(b.get_aPoint()));
    };
    sort(Line_list.begin(), Line_list.end(), cmp);
    std::deque<HalfPlane> deq;
    size_t len = Line_list.size() - 1;
    for (size_t i = 0; i < len; ++i)
    {
        if (eq(Line_list[i].get_k(), Line_list[i + 1].get_k()))
        {
            Line_list.erase(Line_list.begin() + i);
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
        throw "Halfplane can't form a polygon!";
    }
    return Polygon(std::vector<Line>(deq.begin(), deq.end()));
}

double MinDistance(const Polygon &a, const Polygon &b)
{
    if (a.size() < 3 || b.size() < 3)
    {
        throw "too few points for Polygon";
    }
    double ans = GEO_INF;
    for (int i = 0, siz = a.size(); i < siz; ++i)
        ans = std::min(ans, b.MinDis(a(i)));
    for (int i = 0, siz = b.size(); i < siz; ++i)
        ans = std::min(ans, a.MinDis(b(i)));
    return ans;
}

std::pair<Point, Point> Common_Point(const Line &l, const Ellipse &e)
{
    double k = l.get_k();
    Point intercept = l.get_intercept();
    if (k == GEO_INF)
    {
        std::pair<double, double> tmp = e(intercept.x);
        if (eq(tmp.first, GEO_INF))
            return std::make_pair(Point(GEO_INF, GEO_INF), Point(GEO_INF, GEO_INF));
        return std::make_pair(Point(intercept.x, tmp.first), Point(intercept.x, tmp.second));
    }
    else if (eq(k, 0))
    {
        std::pair<double, double> tmp = e[intercept.y];
        if (eq(tmp.first, GEO_INF))
            return std::make_pair(Point(GEO_INF, GEO_INF), Point(GEO_INF, GEO_INF));
        return std::make_pair(Point(tmp.first, intercept.y), Point(tmp.second, intercept.y));
    }
    else
    {
        Vec l_axis = e.get_long_axis(), center = e.get_center();
        double y0 = l(center.x) - center.y;
        double a = e.get_a(), b = e.get_b();
        double a1 = pow((l_axis.x + k * l_axis.y) / a, 2) + pow((k * l_axis.x - l_axis.y) / b, 2);
        double delta = a1 - pow(y0 / a / b, 2);
        if (delta < 0)
            return std::make_pair(Point(GEO_INF, GEO_INF), Point(GEO_INF, GEO_INF));
        delta = sqrt(delta);
        double a2 = ((1 / b / b - 1 / a / a) * l_axis.x * l_axis.y - k * (pow(l_axis.y / a, 2) + pow(l_axis.x / b, 2))) * y0;
        double x1 = (a2 + delta) / a1 + center.x, x2 = (a2 - delta) / a1 + center.x;
        return std::make_pair(Point(x1, l(x1)), Point(x2, l(x2)));
    }
}

//func??????????????????????????????????????????????????????????????????
// P Best_Arg(bool (*cmp)(double,double), func_t op, Line_with_args& l, Ellipse e)
// {
//     if(l.arg_size() == 0)
//     {
//         return std::make_pair(0, op(l, e));
//     }
//     else if(l.arg_size() == 1)
//     {
//         //????????????????????????????????????????????????????????????????????????90???????????????????????????k?????????????????????????????????????????????????????????
//         l.set_args(0);

//         double E = op(l, e);
//         std::cout<<"\n"<<E<<std::endl;
//         P ans = std::make_pair(0, E);
//         double T = T0, old_args = 0;
//         while(T>=Tt)
//         {
//             double new_args = old_args + T * (std::rand() * 2 - RAND_MAX); //???????????????????????????????????????
//             l.set_args(new_args);
//             P new_ = std::make_pair(new_args, op(l, e));
//             //printf("%lf ",l.get_k());
//             if(cmp(new_.second, ans.second))
//             {
//                 ans=new_;
//             }
//             if(cmp(new_.second, E) || std::exp((new_.second - E) / T) > std::rand() / RAND_MAX)//
//             {
//                 E = new_.second;
//                 old_args = new_args;
//             }
//             else
//             {
//                 l.set_args(old_args);
//             }
//             T *= Delta;
//         }
//         int steps = 100;
//         while(steps--)
//         {
//             double new_args = old_args + T * (std::rand() * 2 - RAND_MAX);
//             P new_ = std::make_pair(new_args, op(l, e));
//             if(cmp(new_.second, ans.second))
//             {
//                 ans = new_;
//             }
//         }
//         return ans;
//     }
//     return std::make_pair(0,0);
// }

std::pair<double, double> Best_Arg(double (*op)(double), double max_arg, double min_arg)
{
    static const double Delta = 0.999;
    static const double T0 = 1.0, Tt = 1e-10;

    double T = T0, old_arg = (max_arg + min_arg) / 2, len = max_arg - min_arg;
    double E = op(old_arg);
    std::pair<double, double> ans = std::make_pair(old_arg, E);
    if (eq(len, 0))
        return ans;
    while (T >= Tt)
    {
        double new_arg = old_arg + T * len * (std::rand() * 2 - RAND_MAX) / RAND_MAX; //???????????????????????????????????????
        if (new_arg > max_arg)
            new_arg = max_arg;
        else if (new_arg < min_arg)
            new_arg = min_arg;
        std::pair<double, double> new_ = std::make_pair(new_arg, op(new_arg));
        //printf("%lf ",l.get_k());
        if (new_.second > ans.second)
            ans = new_;
        if (new_.second > E || std::exp(-abs(new_.second - E) / T) > std::rand() / RAND_MAX)
        {
            E = new_.second;
            old_arg = new_arg;
        }
        T *= Delta;
    }
    int steps = 10000;
    while (steps--)
    {
        double new_arg = old_arg + T * len * (std::rand() * 2 - RAND_MAX) / RAND_MAX;
        if (new_arg > max_arg)
            new_arg = max_arg;
        else if (new_arg < min_arg)
            new_arg = min_arg;
        std::pair<double, double> new_ = std::make_pair(new_arg, op(new_arg));
        if (new_.second > ans.second)
            ans = new_;
    }
    return ans;
}
